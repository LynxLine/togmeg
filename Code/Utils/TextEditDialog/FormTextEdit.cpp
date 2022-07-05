//
// Copyright Petrostreamz 2007
//

#include <QtGui>
#include <QtColorPicker>
#include "FormTextEdit.h"

#include "TextEdit.h"

#include "IconSet.h"
#include "AppStyles.h"

QRect FormTextEdit::s_rectGeometry;

class FormTextEdit::Private {
public:
    Private():
         align(0)
        ,prevActionClicked(0L)
        ,isFormatPainterConstant(false)
    {}
    // data
    Qt::Alignment    align;
    QString          initialHtml;
    QList<QString>   initialImages;
    
    QHash<QString, QAction *> actions;
    QActionGroup * listActionGroup;
    QActionGroup * scriptActionGroup;
    QFontComboBox * comboFont;
    QComboBox * comboSize;
    QtColorPicker * colorPicker;
    QComboBox * refsListCombo;
    
    QLineEdit * leftMargin;
    QLineEdit * rightMargin;
    QLineEdit * bottomMargin;
    QLineEdit * topMargin;
    
    QToolBar *       editToolBar;
    QToolBar *       fontToolBar;
    TextEdit *       textEdit;
    QDialogButtonBox * buttons;
    
    // for tracking mouse double clicks
    QTime time; 
    QAction * prevActionClicked;
    bool isFormatPainterConstant;
};

/*!
 */
FormTextEdit::FormTextEdit(QString html, QWidget * parent) 
:QDialog(parent, Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint)
{
    d = new Private;
    setWindowTitle(tr("Edit Rich Text"));
    setAttribute(Qt::WA_MacSmallSize);
    setMinimumSize( 500, 300 );
    setSizeGripEnabled( true );
    createToolBar();
   
    QVBoxLayout * toolBarLayout = new QVBoxLayout();
    toolBarLayout->setSpacing(0);
    toolBarLayout->setMargin(0);
    toolBarLayout->addWidget( d->editToolBar );
    toolBarLayout->addWidget( d->fontToolBar );
    
    d->textEdit  = new TextEdit;
    d->textEdit->setTabChangesFocus(false);

    d->buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel /*| QDialogButtonBox::Help*/);
    
    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(10);
#ifdef Q_WS_MAC
    buttonsLayout->setContentsMargins(15,8,15,12);
#else
    buttonsLayout->setContentsMargins(10,8,10,8);
#endif
    buttonsLayout->addWidget( d->buttons );

    QVBoxLayout * textLayout = new QVBoxLayout();
    textLayout->setContentsMargins(10,7,10,7);
    textLayout->addWidget( d->textEdit );        
    
    QFrame * hline = new QFrame; {
        QPalette palette = hline->palette();
        hline->setFrameStyle(QFrame::HLine | QFrame::Plain);
        palette.setColor(QPalette::WindowText, QColor("#A0A0A0"));
        hline->setPalette(palette);
    }
    
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addLayout( toolBarLayout );
    layout->addLayout( textLayout );
    layout->addWidget( hline );
    layout->addLayout( buttonsLayout );
    setLayout( layout );

    connect( d->buttons, SIGNAL(accepted()),  this, SLOT(accept()));
    connect( d->buttons, SIGNAL(rejected()),  this, SLOT(reject()));
    
    foreach( QAbstractButton * button, d->buttons->buttons()) {
        button->setFocusPolicy(Qt::StrongFocus);
    }
    
    connect( d->textEdit,  SIGNAL(textChanged()), this, SLOT(applyData()));

    if (!s_rectGeometry.isNull() )
        setGeometry( s_rectGeometry );
    
    //
    // Text Edit
    //
    
    connect(d->textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
            this,          SLOT(currentCharFormatChanged(const QTextCharFormat &)));
    connect(d->textEdit, SIGNAL(cursorPositionChanged()),
            this,          SLOT(cursorPositionChanged()));
    connect(d->textEdit, SIGNAL(modeChanged(TextEdit::Mode)), 
            this,          SLOT(formaterPainterModeChanged(TextEdit::Mode)));
            
    d->editToolBar->setFocusPolicy(Qt::NoFocus);
    
    fontChanged(d->textEdit->font());
    colorChanged(d->textEdit->textColor());
    alignmentChanged(d->textEdit->alignment());
    
    connect(d->textEdit->document(), SIGNAL(undoAvailable(bool)),
            d->actions["Undo"], SLOT(setEnabled(bool)));
    connect(d->textEdit->document(), SIGNAL(redoAvailable(bool)),
            d->actions["Redo"], SLOT(setEnabled(bool)));
    
    d->actions["Undo"]->setEnabled(d->textEdit->document()->isUndoAvailable());
    d->actions["Redo"]->setEnabled(d->textEdit->document()->isRedoAvailable());
    
    connect(d->actions["Undo"], SIGNAL(triggered()), d->textEdit, SLOT(undo()));
    connect(d->actions["Redo"], SIGNAL(triggered()), d->textEdit, SLOT(redo()));
    
    d->actions["Cut"]->setEnabled(false);
    d->actions["Copy"]->setEnabled(false);
    
    connect(d->actions["Cut"], SIGNAL(triggered()), d->textEdit, SLOT(cut()));
    connect(d->actions["Copy"], SIGNAL(triggered()), d->textEdit, SLOT(copy()));
    connect(d->actions["Paste"], SIGNAL(triggered()), d->textEdit, SLOT(paste()));
    
    connect(d->textEdit, SIGNAL(copyAvailable(bool)), d->actions["Cut"], SLOT(setEnabled(bool)));
    connect(d->textEdit, SIGNAL(copyAvailable(bool)), d->actions["Copy"], SLOT(setEnabled(bool)));
    
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));
    
    d->textEdit->setFocus();
    
    d->initialHtml = html;
    d->textEdit->setHtml(html);
    
    QTextCursor cursor = d->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    d->textEdit->setTextCursor(cursor);
}

/*!
 */
FormTextEdit::~FormTextEdit()
{
    delete d;
}

TextEdit * FormTextEdit::textEdit() const
{
    return d->textEdit;
}

/*!
 */
void FormTextEdit::createToolBar()
{
    d->editToolBar = new QToolBar;
    d->editToolBar->setIconSize(QSize(20,20));
    d->editToolBar->setStyle( AppStyles::simpleStyle() );

    d->fontToolBar = new QToolBar;
    d->fontToolBar->setIconSize(QSize(20,20));
    d->fontToolBar->setStyle( AppStyles::simpleStyle() );

    QToolBar * tb  = d->editToolBar;
    
    QAction * a;
    a = d->actions["Undo"]  = new QAction(tr("&Undo"), this);
    a = d->actions["Redo"]  = new QAction(tr("&Redo"), this);
    a = d->actions["Cut"]   = new QAction(tr("Cu&t"), this);
    a = d->actions["Copy"]  = new QAction(tr("&Copy"), this);
    a = d->actions["Paste"] = new QAction(tr("&Paste"), this);

    a->setShortcut(QKeySequence::Undo);
    a->setShortcut(QKeySequence::Redo);
    a->setShortcut(QKeySequence::Cut);
    a->setShortcut(QKeySequence::Copy);
    a->setShortcut(QKeySequence::Paste);
    
    tb->addAction(d->actions["Cut"]);
    tb->addAction(d->actions["Copy"]);
    tb->addAction(d->actions["Paste"]);
    d->actions["Cut"]->setVisible(false);
    d->actions["Copy"]->setVisible(false);
    d->actions["Paste"]->setVisible(false);
    
    //d->actions["Paste"]->setEnabled(!QApplication::clipboard()->text().isEmpty());
    
    d->actions["Bold"] = new QAction(EmbIcon("Bold"), tr("&Bold"), this);
    d->actions["Bold"]->setShortcut(Qt::CTRL + Qt::Key_B);
    QFont bold;
    bold.setBold(true);
    d->actions["Bold"]->setFont(bold);
    connect(d->actions["Bold"], SIGNAL(triggered()), this, SLOT(textBold()));
    tb->addAction(d->actions["Bold"]);
    d->actions["Bold"]->setCheckable(true);
    
    d->actions["Italic"] = new QAction(EmbIcon("Italic"), tr("&Italic"), this);
    d->actions["Italic"]->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    d->actions["Italic"]->setFont(italic);
    connect(d->actions["Italic"], SIGNAL(triggered()), this, SLOT(textItalic()));
    tb->addAction(d->actions["Italic"]);
    d->actions["Italic"]->setCheckable(true);
    
    d->actions["Underline"] = new QAction(EmbIcon("Underline"), tr("&Underline"), this);
    d->actions["Underline"]->setShortcut(Qt::CTRL + Qt::Key_U);
    QFont underline;
    underline.setUnderline(true);
    d->actions["Underline"]->setFont(underline);
    connect(d->actions["Underline"], SIGNAL(triggered()), this, SLOT(textUnderline()));
    tb->addAction(d->actions["Underline"]);
    d->actions["Underline"]->setCheckable(true);
    tb->addSeparator();

    QActionGroup *g_halign = new QActionGroup(this);
    connect(g_halign, SIGNAL(triggered(QAction *)), this, SLOT(textHAlign(QAction *)));
    
    d->actions["AlignLeft"] = new QAction(EmbIcon("AlignTextLeft"), tr("&Left"), g_halign);
    d->actions["AlignLeft"]->setShortcut(Qt::CTRL + Qt::Key_L);
    d->actions["AlignLeft"]->setCheckable(true);
    
    d->actions["AlignHCenter"] = new QAction(EmbIcon("AlignTextCenter"), tr("C&enter"), g_halign);
    d->actions["AlignHCenter"]->setShortcut(Qt::CTRL + Qt::Key_E);
    d->actions["AlignHCenter"]->setCheckable(true);
    
    d->actions["AlignRight"] = new QAction(EmbIcon("AlignTextRight"), tr("&Right"), g_halign);
    d->actions["AlignRight"]->setShortcut(Qt::CTRL + Qt::Key_R);
    d->actions["AlignRight"]->setCheckable(true);
    
    d->actions["AlignJustify"] = new QAction(EmbIcon("AlignTextJustify"), tr("&Justify"), g_halign);
    d->actions["AlignJustify"]->setShortcut(Qt::CTRL + Qt::Key_J);
    d->actions["AlignJustify"]->setCheckable(true);
    tb->addActions(g_halign->actions());
    tb->addSeparator();

    QActionGroup *g_valign = new QActionGroup(this);
    connect(g_valign, SIGNAL(triggered(QAction *)), this, SLOT(textVAlign(QAction *)));

    d->actions["AlignTop"] = new QAction(EmbIcon("AlignTextTop"), tr("Top"), g_valign);
    d->actions["AlignTop"]->setCheckable(true);

    d->actions["AlignVCenter"] = new QAction(EmbIcon("AlignTextVCenter"), tr("VCenter"), g_valign);
    d->actions["AlignVCenter"]->setCheckable(true);

    d->actions["AlignBottom"] = new QAction(EmbIcon("AlignTextBottom"), tr("Bottom"), g_valign);
    d->actions["AlignBottom"]->setCheckable(true);

    tb->addActions(g_valign->actions());
    tb->addSeparator();

    d->listActionGroup = new QActionGroup(this);
    d->actions["DiscList"]     = new QAction(tr("Bullet List (Disc)"), d->listActionGroup);
    d->actions["CircleList"]   = new QAction(tr("Bullet List (Circle)"), d->listActionGroup);
    d->actions["SquareList"]   = new QAction(tr("Bullet List (Square)"), d->listActionGroup);
    d->actions["OrderedList"]  = new QAction(tr("1. List"), d->listActionGroup);
    d->actions["AlphaList"]    = new QAction(tr("A. List"), d->listActionGroup);
    d->actions["AlphaLowList"] = new QAction(tr("a. List"), d->listActionGroup);
    
    QMenu * listMenu = new QMenu(this);
    listMenu->addActions( d->listActionGroup->actions() );
    foreach(QAction * a, d->listActionGroup->actions()) {
        connect(a, SIGNAL(triggered()), this, SLOT(createList()));
        a->setCheckable(true);
    }
    d->actions["DiscList"]->setChecked(true);
    
    d->actions["List"] = new QAction(EmbIcon("CreateList"), tr("List"), this);
    d->actions["List"]->setMenu( listMenu );
    connect(d->actions["List"], SIGNAL(triggered()), this, SLOT(createList()));
    tb->addAction( d->actions["List"] );
    
    d->actions["Indent"] = new QAction(EmbIcon("Indent"), tr("Indent"), this);
    tb->addAction( d->actions["Indent"] );
    connect(d->actions["Indent"], SIGNAL(triggered()), this, SLOT(indentText()));

    d->actions["UnIndent"] = new QAction(EmbIcon("Unindent"), tr("Unindent"), this);
    tb->addAction( d->actions["UnIndent"] );
    connect(d->actions["UnIndent"], SIGNAL(triggered()), this, SLOT(unindentText()));
    
    tb->addSeparator();

    d->scriptActionGroup = new QActionGroup(this);
    d->scriptActionGroup->setExclusive(false);
    
    d->actions["SubScript"] = new QAction(EmbIcon("SubScript"), tr("Sub Script"), d->scriptActionGroup);
    d->actions["SubScript"]->setCheckable(true);
    connect(d->actions["SubScript"], SIGNAL(triggered()), this, SLOT(textSubScript()));
    
    d->actions["SuperScript"] = new QAction(EmbIcon("SuperScript"), tr("Super Script"), d->scriptActionGroup);
    d->actions["SuperScript"]->setCheckable(true);
    connect(d->actions["SuperScript"], SIGNAL(triggered()), this, SLOT(textSuperScript()));

    tb->addActions( d->scriptActionGroup->actions() );
    tb->addSeparator();

    d->colorPicker = new QtColorPicker;
    d->colorPicker->setStandardColors();
    connect(d->colorPicker, SIGNAL(colorChanged(const QColor &)),
            this, SLOT(setTextColor(const QColor &)));
    
    tb->addWidget( d->colorPicker );
    setCurrentAlignment(Qt::AlignCenter);
    tb  = d->fontToolBar;
            
    d->comboFont = new QFontComboBox(tb);
    d->comboFont->setMaximumWidth(200);
    tb->addWidget(d->comboFont);
    connect(d->comboFont, SIGNAL(activated(const QString &)),
            this,           SLOT(textFamily(const QString &)));
    
    d->comboSize = new QComboBox(tb);
    tb->addWidget(d->comboSize);
    d->comboSize->setEditable(true);
    
    tb->addSeparator();
    
    QWidget  *margins = new QWidget();
    QIntValidator *positive = new QIntValidator(0, 10000, margins);

    const int max_width = 40;
    d->leftMargin = new QLineEdit();
    d->leftMargin->setMaximumWidth(max_width);
    d->leftMargin->setValidator(positive);
    connect(d->leftMargin, SIGNAL(textChanged(const QString &)),
            this,          SLOT(leftMarginChanged(const QString &)));

    d->rightMargin = new QLineEdit();
    d->rightMargin->setMaximumWidth(max_width);
    d->rightMargin->setValidator(positive);
    connect(d->rightMargin, SIGNAL(textChanged(const QString &)),
            this,           SLOT(rightMarginChanged(const QString &)));

    d->bottomMargin = new QLineEdit();
    d->bottomMargin->setMaximumWidth(max_width);
    d->bottomMargin->setValidator(positive);
    connect(d->bottomMargin, SIGNAL(textChanged(const QString &)),
            this,            SLOT(bottomMarginChanged(const QString &)));

    d->topMargin = new QLineEdit();
    d->topMargin->setMaximumWidth(max_width);
    d->topMargin->setValidator(positive);
    connect(d->topMargin, SIGNAL(textChanged(const QString &)),
            this,         SLOT(topMarginChanged(const QString &)));
    
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setSpacing(0);
    vbox->setMargin(0);
    vbox->addWidget(d->topMargin);
    vbox->addWidget(d->bottomMargin);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setSpacing(0);
    hbox->setMargin(0);
    hbox->addWidget(d->leftMargin);
    hbox->addLayout(vbox);
    hbox->addWidget(d->rightMargin);
    
    margins->setLayout(hbox);
    margins->setMaximumWidth(3*max_width);

    tb->addWidget(margins);
    
    tb->addSeparator(); 
    
    d->actions["FormatPainter"] = new QAction(EmbIcon("FormatPainter"), tr("Format Painter"), this);
    d->actions["FormatPainter"]->setCheckable(true);
    d->actions["FormatPainter"]->setChecked(false);
    tb->addAction( d->actions["FormatPainter"] );
    connect(d->actions["FormatPainter"], SIGNAL(triggered(bool)), this, SLOT(setFormatPainterMode(bool)));
    
    QFontDatabase db;
    foreach(int size, db.standardSizes())
        d->comboSize->addItem(QString::number(size));
    
    connect(d->comboSize, SIGNAL(activated(const QString &)),
            this,           SLOT(textSize(const QString &)));
    d->comboSize->setCurrentIndex(d->comboSize->findText(QString::number(QApplication::font().pointSize())));
}

QString FormTextEdit::htmlText() const 
{ 
    return d->textEdit->toHtml(); 
}

/*!
 */
void FormTextEdit::setControlData()
{
    applyData();
}

/*!
 */
void FormTextEdit::accept()
{
    applyData();
    QDialog::accept();
}

/*!
 */
void FormTextEdit::reject()
{
    d->textEdit->setHtml( d->initialHtml );
    applyData();
    
    QDialog::reject();
}

/*!
 */
void FormTextEdit::setAlignment(Qt::Alignment a)
{
    if (a & Qt::AlignTop) {
        if (!d->actions["AlignTop"]->isChecked())
            d->actions["AlignTop"]->setChecked(true);
    }
    else if (a & Qt::AlignBottom) {
        if (!d->actions["AlignBottom"]->isChecked())
            d->actions["AlignBottom"]->setChecked(true);
    }
    else /* if (a & Qt::AlignVCenter) */ {
        if (!d->actions["AlignVCenter"]->isChecked())
            d->actions["AlignVCenter"]->setChecked(true);
    }

    setControlData();
}

void FormTextEdit::setDefaultFont(QFont f)
{
    d->textEdit->document()->setDefaultFont(f);
    fontChanged(f);
}

/*!
 */
void FormTextEdit::applyData()
{
    emit contentChanged();
}

/*!
 Initializes controls in show event.
 */
void FormTextEdit::showEvent( QShowEvent * pEvent )
{
    if (pEvent->type() == QEvent::Show && !pEvent->spontaneous() ) {
        setControlData();
    }
}

/*!
 Saves geometry on close.
 */
void FormTextEdit::closeEvent( QCloseEvent * pEvent )
{
    if (!pEvent->spontaneous() )
        s_rectGeometry = geometry();
}

/*!
 Saves geometry on hide event.
 */
void FormTextEdit::hideEvent( QHideEvent * pEvent )
{
    if (!pEvent->spontaneous() )
        s_rectGeometry = geometry();
}

//
// Text formatting
//

void FormTextEdit::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(d->actions["Bold"]->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void FormTextEdit::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(d->actions["Underline"]->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void FormTextEdit::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(d->actions["Italic"]->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void FormTextEdit::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void FormTextEdit::leftMarginChanged(const QString &m) {
    QTextBlockFormat fmt;
    fmt.setLeftMargin(m.toDouble() );
    mergeBlockFormat(fmt);
}

void FormTextEdit::rightMarginChanged(const QString &m) {
    QTextBlockFormat fmt;
    fmt.setRightMargin(m.toDouble() );
    mergeBlockFormat(fmt);
}

void FormTextEdit::bottomMarginChanged(const QString &m) {
    QTextBlockFormat fmt;
    fmt.setBottomMargin(m.toDouble() );
    mergeBlockFormat(fmt);
}

void FormTextEdit::topMarginChanged(const QString &m) {
    QTextBlockFormat fmt;
    fmt.setTopMargin(m.toDouble() );
    mergeBlockFormat(fmt);
}

void FormTextEdit::textSize(const QString &p)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(p.toFloat());
    mergeFormatOnWordOrSelection(fmt);
}

void FormTextEdit::createList()
{
    QTextListFormat::Style style = QTextListFormat::ListDisc;
    if (d->actions["DiscList"]->isChecked()) style = QTextListFormat::ListDisc;
    else if (d->actions["CircleList"]->isChecked()) style = QTextListFormat::ListCircle;
    else if (d->actions["SquareList"]->isChecked()) style = QTextListFormat::ListSquare;
    else if (d->actions["OrderedList"]->isChecked()) style = QTextListFormat::ListDecimal;
    else if (d->actions["AlphaLowList"]->isChecked()) style = QTextListFormat::ListLowerAlpha;
    else if (d->actions["AlphaList"]->isChecked()) style = QTextListFormat::ListUpperAlpha;
    
    textStyle(style);
}

void FormTextEdit::textStyle(QTextListFormat::Style style)
{
    QTextCursor cursor = d->textEdit->textCursor();
    cursor.beginEditBlock();
    
    QTextBlockFormat blockFmt = cursor.blockFormat();
    QTextListFormat listFmt;
    
    if (cursor.currentList()) {
        listFmt = cursor.currentList()->format();
    } else {
        listFmt.setIndent(blockFmt.indent() + 1);
        blockFmt.setIndent(0);
        cursor.setBlockFormat(blockFmt);
    }
    
    listFmt.setStyle(style);
    cursor.createList(listFmt);
    cursor.endEditBlock();
}

void FormTextEdit::indentText()
{
    QTextCursor cursor = d->textEdit->textCursor();        
    if (cursor.currentList()) {
        QTextListFormat listFmt = cursor.currentList()->format();
        listFmt.setIndent(listFmt.indent() + 1);
        
        cursor.beginEditBlock();
        cursor.createList(listFmt);
        cursor.endEditBlock();
    } 
    else {
        
        
        int start = cursor.anchor();
        int end = cursor.position();
        if (start >end) {
            start = cursor.position();
            end = cursor.anchor();
        }
        
        QList<QTextBlock> blocks;
        QTextBlock b = d->textEdit->document()->begin();
        while (b.isValid()) {
            b = b.next();
            if ((b.position() >=start &&
                 b.position()+b.length() <=end) ||
                b.contains(start) || b.contains(end)) {
                blocks << b; 
            }
        }
        
        foreach(QTextBlock b, blocks) {
            QTextCursor c(b);
            QTextBlockFormat bf = c.blockFormat();
            bf.setIndent( bf.indent()+1 );
            c.setBlockFormat(bf);
        }
    }
}

void FormTextEdit::unindentText()
{
    QTextCursor cursor = d->textEdit->textCursor();
    QTextList * list = cursor.currentList();
    if (list && list->format().indent() >1) {
            
        int start = cursor.anchor();
        int end = cursor.position();
        if (start >end) {
            start = cursor.position();
            end = cursor.anchor();
        }
        
        QList<QTextBlock> blocks;
        QTextBlock b = d->textEdit->document()->begin();
        while (b.isValid()) {
            b = b.next();
            if ((b.position() >=start &&
                b.position()+b.length() <=end) ||
                b.contains(start) || b.contains(end)) {
                blocks << b; 
            }
        }
        
        
        foreach(QTextBlock b, blocks) {
            if (b.textList()) {
                QTextCursor bc(b);
                QTextBlock pb = b.previous();
                while (pb.isValid()) {
                    QTextCursor pc(pb);
                    QTextList * pl = pc.currentList();
                    if (!pl) break; // do nothing
                    
                    if (pl->format().indent() < list->format().indent()) {
                        QTextBlockFormat bf = pb.blockFormat();
                        b.textList()->remove(b);
                        bc.setBlockFormat(bf);
                        pl->add(b);
                        break;
                    }
                    
                    pb = pb.previous();
                }
            }
        }
    }    
    else { // no list
        
        int start = cursor.anchor();
        int end = cursor.position();
        if (start >end) {
            start = cursor.position();
            end = cursor.anchor();
        }
        
        QList<QTextBlock> blocks;
        QTextBlock b = d->textEdit->document()->begin();
        while (b.isValid()) {
            b = b.next();
            if ((b.position() >=start &&
                 b.position()+b.length() <=end) ||
                b.contains(start) || b.contains(end)) {
                blocks << b; 
            }
        }
        
        foreach(QTextBlock b, blocks) {
            QTextCursor c(b);
            QTextBlockFormat bf = c.blockFormat();
            if (bf.indent() >0) {
                bf.setIndent( bf.indent()-1 );
                c.setBlockFormat(bf);
            }
        }
        
    }
}

void FormTextEdit::setTextColor(const QColor & col)
{
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
    d->textEdit->setFocus();
}

void FormTextEdit::textSubScript()
{
    QTextCharFormat fmt;
    fmt.setVerticalAlignment(d->actions["SubScript"]->isChecked() ?
                             QTextCharFormat::AlignSubScript : QTextCharFormat::AlignNormal );
    mergeFormatOnWordOrSelection(fmt);
}

void FormTextEdit::textSuperScript()
{
    QTextCharFormat fmt;
    fmt.setVerticalAlignment(d->actions["SuperScript"]->isChecked() ?
                             QTextCharFormat::AlignSuperScript : QTextCharFormat::AlignNormal );
    mergeFormatOnWordOrSelection(fmt);
}

void FormTextEdit::textHAlign(QAction *a)
{
    if (a == d->actions["AlignLeft"])
        d->textEdit->setAlignment(Qt::AlignLeft);
    else if (a == d->actions["AlignHCenter"])
        d->textEdit->setAlignment(Qt::AlignHCenter);
    else if (a == d->actions["AlignRight"])
        d->textEdit->setAlignment(Qt::AlignRight);
    else if (a == d->actions["AlignJustify"])
        d->textEdit->setAlignment(Qt::AlignJustify);
}

void FormTextEdit::setCurrentAlignment(Qt::Alignment a)
{
    if (d->align == a) return;

    d->align = a;
    setAlignment(d->align);
}

void FormTextEdit::textVAlign(QAction *a)
{
    Qt::Alignment al = d->align;
    al &= Qt::AlignHorizontal_Mask;
    if (a == d->actions["AlignTop"]) {
        al |= Qt::AlignTop;
        setCurrentAlignment(al);
    }
    else if (a == d->actions["AlignVCenter"]) {
        al |= Qt::AlignVCenter;
        setCurrentAlignment(al);
    }
    else if (a == d->actions["AlignBottom"]) {
        al |= Qt::AlignBottom;
        setCurrentAlignment(al);
    }
}

void FormTextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
    textScriptChanged(format.verticalAlignment());
    styleChanged();
}

void FormTextEdit::cursorPositionChanged()
{
    alignmentChanged(d->textEdit->alignment());
    styleChanged();
    marginsChanged();
}

void FormTextEdit::clipboardDataChanged()
{
    //d->actions["Paste"]->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

void FormTextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = d->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    d->textEdit->mergeCurrentCharFormat(format);
}

void FormTextEdit::mergeBlockFormat(const QTextBlockFormat &format)
{
    QTextCursor cursor = d->textEdit->textCursor();
    cursor.mergeBlockFormat(format);
}

void FormTextEdit::fontChanged(const QFont &f)
{
    d->comboFont->setCurrentIndex(d->comboFont->findText(QFontInfo(f).family()));
    d->comboSize->setCurrentIndex(d->comboSize->findText(QString::number(f.pointSize())));
    d->actions["Bold"]->setChecked(f.bold());
    d->actions["Italic"]->setChecked(f.italic());
    d->actions["Underline"]->setChecked(f.underline());
}

void FormTextEdit::marginsChanged() 
{
    QTextCursor cursor = d->textEdit->textCursor();
    QTextBlockFormat blockFmt = cursor.blockFormat();

    d->leftMargin->blockSignals(true);
    d->leftMargin->setText(QString::number(blockFmt.leftMargin(), 'f', 0));
    d->leftMargin->blockSignals(false);

    d->rightMargin->blockSignals(true);
    d->rightMargin->setText(QString::number(blockFmt.rightMargin(), 'f', 0));
    d->rightMargin->blockSignals(false);

    d->bottomMargin->blockSignals(true);
    d->bottomMargin->setText(QString::number(blockFmt.bottomMargin(), 'f', 0));
    d->bottomMargin->blockSignals(false);

    d->topMargin->blockSignals(true);
    d->topMargin->setText(QString::number(blockFmt.topMargin(), 'f', 0));
    d->topMargin->blockSignals(false);
}

void FormTextEdit::styleChanged()
{
    QTextCursor cursor = d->textEdit->textCursor();
    QTextBlockFormat blockFmt = cursor.blockFormat();
    d->listActionGroup->blockSignals(true);
    if (cursor.currentList()) {
        QTextListFormat listFmt = cursor.currentList()->format();
        if (listFmt.style() == QTextListFormat::ListDisc) d->actions["DiscList"]->setChecked(true);
        else if (listFmt.style() == QTextListFormat::ListCircle) d->actions["CircleList"]->setChecked(true);
        else if (listFmt.style() == QTextListFormat::ListSquare) d->actions["SquareList"]->setChecked(true);
        else if (listFmt.style() == QTextListFormat::ListDecimal) d->actions["OrderedList"]->setChecked(true);
        else if (listFmt.style() == QTextListFormat::ListLowerAlpha) d->actions["AlphaLowList"]->setChecked(true);
        else if (listFmt.style() == QTextListFormat::ListUpperAlpha) d->actions["AlphaList"]->setChecked(true);
    }
    d->listActionGroup->blockSignals(false);
}

void FormTextEdit::colorChanged(const QColor &c)
{
    d->colorPicker->blockSignals(true);
    d->colorPicker->setCurrentColor(c);
    d->colorPicker->blockSignals(false);
}

void FormTextEdit::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft) {
        d->actions["AlignLeft"]->setChecked(true);
    } else if (a & Qt::AlignHCenter) {
        d->actions["AlignHCenter"]->setChecked(true);
    } else if (a & Qt::AlignRight) {
        d->actions["AlignRight"]->setChecked(true);
    } else if (a & Qt::AlignJustify) {
        d->actions["AlignJustify"]->setChecked(true);
    }
}

void FormTextEdit::textScriptChanged(QTextCharFormat::VerticalAlignment v)
{
    d->actions["SubScript"]->setChecked(v == QTextCharFormat::AlignSubScript );
    d->actions["SuperScript"]->setChecked( v == QTextCharFormat::AlignSuperScript );
}

void FormTextEdit::setFormatPainterMode(bool f)
{
    bool clicked = testDoubleClick(d->actions["FormatPainter"]);
    d->isFormatPainterConstant = clicked;
    d->textEdit->setMode((f || clicked) ? TextEdit::FormatPainter : TextEdit::Default);
            
    d->actions["FormatPainter"]->blockSignals(true);
    d->actions["FormatPainter"]->setChecked(clicked || f);
    d->actions["FormatPainter"]->blockSignals(false);
}

void FormTextEdit::formaterPainterModeChanged(TextEdit::Mode m)
{
    if (m == TextEdit::Default) {
        d->actions["FormatPainter"]->blockSignals(true);
        d->actions["FormatPainter"]->setChecked(d->isFormatPainterConstant);
        d->actions["FormatPainter"]->blockSignals(false);
        
        if (d->isFormatPainterConstant)
            d->textEdit->setMode(TextEdit::FormatPainter);
    }
}

bool FormTextEdit::testDoubleClick(QAction * a)
{
    int msecs = d->time.restart();
    bool f = a && a == d->prevActionClicked && msecs <= qApp->doubleClickInterval();
    
    if( d->prevActionClicked && d->prevActionClicked->isChecked() ) {
        d->prevActionClicked->setChecked( false );
    }
    
    d->prevActionClicked = a;        
    return f;
}

void FormTextEdit::initReferences(QStringList refs)
{
    if (!refs.count())
        return;
    QToolBar * tb = d->fontToolBar;
    tb->addSeparator();
    QWidget * refsWidget = new QWidget();
    QHBoxLayout * hbLayout = new QHBoxLayout();
    QLabel * refsLabel = new QLabel();
    d->refsListCombo = new QComboBox(tb);
    d->refsListCombo->setMaximumWidth(150);
    refsLabel->setText("Ref:");
    hbLayout->setContentsMargins(0,0,0,0);
    hbLayout->setSpacing(0);
    hbLayout->addWidget(refsLabel);
    hbLayout->addWidget(d->refsListCombo);
    refsWidget->setLayout(hbLayout);
    tb->addWidget(refsWidget);
    d->refsListCombo->addItem("");
    d->refsListCombo->setCurrentIndex(0);
    foreach (QString ref, refs)
        d->refsListCombo->addItem(ref);
    connect(d->refsListCombo, SIGNAL(activated(int)), this, SLOT(insertReference(int)));
    if (tb->parentWidget())
        tb->parentWidget()->adjustSize();
}

void FormTextEdit::insertReference(int idx)
{
    if (!d->textEdit || !d->refsListCombo)
        return;
    QTextCursor cursor = d->textEdit->textCursor();
    cursor.insertText(d->refsListCombo->itemText(idx));
    d->refsListCombo->setCurrentIndex(0);
    d->textEdit->setFocus();
}
