
#include <QtGui>
#include "TogMeg.h"
#include "TogMegWindow.h"

#include "QAModel.h"
#include "QAEditor.h"
#include "QADelegate.h"

#include "VHeader.h"
#include "AppStyles.h"
#include "ColumnSelectorButton.h"

class QAEditor::Private
{
public:
    NextItemMode mode;
    QPointer<QAModel> model;
    QPointer<QMenu> contextMenu;
    bool in_double_click;
};

QAEditor::QAEditor(QAModel * model, QWidget * parent)
:QTableView(parent)
{
    d = new Private;
    d->model = model;
    d->in_double_click = false;
    d->mode = NextItemMode(-1); //undefined;

    setVerticalHeader(new QVHeaderView(Qt::Vertical, this));

    QFontMetrics fm(font());
    setFrameStyle(QFrame::NoFrame);
    setContentsMargins(0,0,0,0);
    setLineWidth(0);

    setStyleSheet("QTableCornerButton::section {background-color: #e0e0e0;"\
                  "border: 1px solid #a0a0a0; border-top: 0px; border-left: 0px;}");

#ifdef Q_WS_MAC
    horizontalHeader()->setDefaultSectionSize(fm.height()+2);
    verticalHeader()->setDefaultSectionSize(fm.height()+2);
#else
    horizontalHeader()->setDefaultSectionSize(fm.height()+6);
    verticalHeader()->setDefaultSectionSize(fm.height()+6);
#endif

    setMouseTracking(true);
    setSortingEnabled(false);
    setAlternatingRowColors(true);
    setAttribute(Qt::WA_MacShowFocusRect, false);
    setEditTriggers(QAbstractItemView::DoubleClicked);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

#ifdef Q_WS_MAC
    horizontalHeader()->setFixedHeight(QFontMetrics(AppStyles::systemFont()).height()+3);
    horizontalHeader()->setStyleSheet(AppStyles::mac_horHeaderStyle);
    setFrameStyle(QFrame::NoFrame);
    setContentsMargins(0,0,0,0);
    setLineWidth(0);
#endif

    QSize size;
    QStyleOptionHeader opt;
    size = style()->sizeFromContents(QStyle::CT_HeaderSection, &opt, size, this);        
#ifdef Q_WS_MAC
    size.setHeight(QFontMetrics(AppStyles::systemFont()).height()+3);
#endif
    
    ColumnSelectorButton * b_columns = new ColumnSelectorButton(horizontalHeader());
    b_columns->setFixedHeight(size.height());
    b_columns->setPixmap(QPixmap());
    addScrollBarWidget(b_columns, Qt::AlignTop);
    
    setEditTriggers(QAbstractItemView::EditKeyPressed |
                    QAbstractItemView::AnyKeyPressed);
    setMode(QAQAMode);

    TaskEditorItemDelegate * delegate = new TaskEditorItemDelegate(this);
    connect(delegate, SIGNAL(gotoNextItem()),  this, SLOT(gotoNextByMode()));
    connect(delegate, SIGNAL(returnPressed()), this, SLOT(editNextByMode()));
    connect(delegate, SIGNAL(editAboveItem(int)), this, SLOT(editAbove(int)));
    connect(delegate, SIGNAL(editBelowItem(int)), this, SLOT(editBelow(int)));
    connect(this, SIGNAL(setPosInLineEdit(int)), delegate, SLOT(setPosInLineEdit(int)));
    setItemDelegate( delegate );

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
            this, SLOT(activateContextMenu(const QPoint &)));

    d->contextMenu = new QMenu(this);
    //d->contextMenu->addAction(tr("Add new Question"));
    //d->contextMenu->addSeparator();
    d->contextMenu->addAction(tr("&Remove"),
                              this, SLOT(removeSelection())/*, QKeySequence("Backspace")*/);

    setModel( model );

    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    horizontalHeader()->setResizeMode(QAModel::ColQ, QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(QAModel::ColA, QHeaderView::Stretch);
    
    connect(this, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(onDoubleClick(QModelIndex)), Qt::QueuedConnection);

    connect(model, SIGNAL(modelAboutToBeReset()), this, SLOT(modelAboutToBeReset()));
    toFirstRow();
}

QAEditor::~QAEditor()
{
    delete d;
}

QAEditor::NextItemMode QAEditor::mode() const { return d->mode; }

QModelIndex QAEditor::next(QModelIndex i) const {
    if ( !i.isValid() ) return i;
    int row = i.row();

    if (mode() == QAQAMode) {
        if (i.column() == QAModel::ColQ)
            return model()->index(row, QAModel::ColA);

        if (i.column() == QAModel::ColA) {
            if (row < model()->rowCount()-1) row++;
            return model()->index(row, QAModel::ColQ);
        }
    }
    else if (mode() == QQAAMode) {
        if (row < model()->rowCount()-1) row++;
        return model()->index(row, i.column());
    }

    return i;
}

QModelIndex QAEditor::prev(QModelIndex i) const {
    if (!i.isValid() ) return i;
    int row = i.row();

    if (i.column() == QAModel::ColQ) {
        if (row > 1) row--;
        return model()->index(row, QAModel::ColA);
    }
    else if (i.column() == QAModel::ColA)
        return model()->index(row, QAModel::ColQ);

    return i;
}

void QAEditor::setMode(NextItemMode m)
{
    if (d->mode == m) return;
    d->mode = m;
}

void QAEditor::activateContextMenu(const QPoint & pos)
{
    QModelIndex index = currentIndex();
    if ( !index.isValid() ) return;

    d->contextMenu->popup( viewport()->mapToGlobal(pos) );
}

void QAEditor::toFirstRow()
{
    if (d->model->rowCount() >1) {
        QModelIndex index = model()->index(1, QAModel::ColQ);
        QMetaObject::invokeMethod(this,
                                  "setCurrentIndex",
                                  Qt::QueuedConnection,
                                  Q_ARG(QModelIndex, index));
        QMetaObject::invokeMethod(this,
                                  "edit",
                                  Qt::QueuedConnection,
                                  Q_ARG(QModelIndex, index));
    }
}

void QAEditor::addNewEntry() {
    setCurrentIndex(d->model->addNewEntry());
    edit(currentIndex());
}

void QAEditor::removeSelection()
{
    QSet<int> rows_set;
    foreach(QModelIndex mi, selectionModel()->selectedRows()) {
        rows_set << mi.row();
    }

    QList<int> rows = rows_set.toList();
    qSort(rows.begin(), rows.end(), qGreater<int>());

    foreach(int row, rows) {
        QModelIndex index = d->model->index(row,0);
        d->model->removeEntry(index);
    }
    scrollTo(currentIndex());
}

void QAEditor::swapQA()
{
    QModelIndex index = currentIndex();
    d->model->swapQA( index );
    scrollTo(currentIndex());
}

void QAEditor::editNextByMode()
{
    QModelIndex mi = next(currentIndex());
    setCurrentIndex(mi);
    edit(mi);
}

void QAEditor::gotoNextByMode() {
    setCurrentIndex(next(currentIndex()));
}

void QAEditor::gotoNext()
{
    NextItemMode saved = d->mode;
    d->mode = QAQAMode;
    setCurrentIndex(next(currentIndex()));
    d->mode = saved;
}

void QAEditor::gotoPrev()
{
    NextItemMode saved = d->mode;
    d->mode = QAQAMode;
    setCurrentIndex(prev(currentIndex()));
    d->mode = saved;
}

void QAEditor::gotoPrevByMode()
{

}

void QAEditor::editNext()
{
    QModelIndex old = currentIndex();
    gotoNext();
    if (old == currentIndex()) return;
    edit(currentIndex());
}

void QAEditor::editAbove(int pos)
{
    QModelIndex mi = currentIndex();
    if (!mi.isValid()) return;

    int row = mi.row() -1;
    int col = mi.column();
    if (row <1) return;

    QModelIndex next = model()->index( row, col );
    setCurrentIndex(next);
    edit(next);
    emit setPosInLineEdit(pos);
}

void QAEditor::editBelow(int pos)
{
    QModelIndex mi = currentIndex();
    if (!mi.isValid()) return;

    int row = mi.row() +1;
    int col = mi.column();
    if (row > model()->rowCount()-1) return;

    QModelIndex next = model()->index( row, col );
    setCurrentIndex(next);
    edit(next);
    emit setPosInLineEdit(pos);
}

void QAEditor::editPrev() {
    QModelIndex mi = prev(currentIndex());
    setCurrentIndex(mi);
    edit(mi);
}

void QAEditor::closeEditor(QWidget * e, QAbstractItemDelegate::EndEditHint hint)
{
    if (hint == QAbstractItemDelegate::EditNextItem) {
        QTableView::closeEditor(e, QAbstractItemDelegate::NoHint);
        editNext();
    }
    else if (hint == QAbstractItemDelegate::EditPreviousItem) {
        QTableView::closeEditor(e, QAbstractItemDelegate::NoHint);
        editPrev();
    }
    else if (hint == QAbstractItemDelegate::RevertModelCache)
        QTableView::closeEditor(e, QAbstractItemDelegate::RevertModelCache);
    else QTableView::closeEditor(e, hint);
}

void QAEditor::modelAboutToBeReset()
{
    setCurrentIndex(QModelIndex()); 
}

// Operations:

void QAEditor::cut()
{
    copy();
    clearSelection();
}

/*!
 Copys selection in system clipboard.
 It uses html-type mime data, so it can be affected
 to MS Excel, MS Word
 */
void QAEditor::copy()
{
    int top = d->model->rowCount();
    int bottom = -1;
    int left = d->model->columnCount();
    int right = -1;

    QList<QModelIndex> indexes = selectionModel()->selection().indexes();
    foreach(QModelIndex index, indexes) {
        top    = qMin(top,    index.row());
        bottom = qMax(bottom, index.row());
        left   = qMin(left,   index.column());
        right  = qMax(right,  index.column());
    }

    if ( right <0 ) return;
    if ( bottom <0 ) return;

    QString data = "<!--StartFragment-->\n";
    data += "<table>";

    if (selectionModel()->isColumnSelected(left, QModelIndex())) {
        data += "<tr>\n";
        for (int column=left;column<=right;column++) {
            QVariant v = model()->headerData(column, Qt::Horizontal, Qt::DisplayRole);
            data += "  <td>";
            data += v.toString();
            data += "</td>\n";
        }
        data += "</tr>\n";
    }

    for (int row=top;row<=bottom;row++) {
        data += "<tr>\n";
        for (int column=left;column<=right;column++) {
            QVariant v = d->model->data( d->model->index(row,column) );
            if ( v.canConvert(QVariant::Double) ) data += "  <td x:num>";
            else data += "  <td>";

            data += v.toString();
            data += "</td>\n";
        }
        data += "</tr>\n";
    }

    data += "</table>";
    data += "<!--EndFragment-->\n";

    QString plainData;
    for (int row=top;row<=bottom;row++) {
        for (int column=left;column<=right;column++) {
            QVariant v = d->model->data( d->model->index(row,column) );
            plainData += v.toString();
            if (column < right)
                plainData += "\t";
        }
        if (row < bottom)
            plainData += "\n";
    }

    QMimeData * mimeData = new QMimeData;
    mimeData->setText( plainData );
    mimeData->setHtml( data );
    QApplication::clipboard()->setMimeData(mimeData);
}

/*!
 Paste method uses clipboard mimedata -
 text/html to parse html tables, tr, td
 to receive appropriate cell data.
 */
void QAEditor::paste()
{
    QTime t = QTime::currentTime();

    QString subtype = "html";
    QString data = QApplication::clipboard()->text(subtype);
    if ( data.isEmpty() ) {
        data = QApplication::clipboard()->text();
        if (!data.isEmpty())
            pastePlainData( data );

        return;
    }

    int beginPos = -1, endPos = -1;
    QString begin = "<!--StartFragment-->";
    QString end   = "<!--EndFragment-->";

    beginPos = data.indexOf(begin);
    if ( (beginPos = data.indexOf(begin)) <0 ) return;
    if ( (endPos   = data.indexOf(end, beginPos)) <0 ) return;

    data = data.mid(beginPos +begin.length(), endPos -beginPos -begin.length());

    QModelIndex current = currentIndex();
    if (!current.isValid())
        current = d->model->index(1,0);

    if (selectionModel()->hasSelection()) {
        int top = d->model->rowCount();
        int bottom = -1;
        int left = d->model->columnCount();
        int right = -1;

        QList<QModelIndex> indexes = selectionModel()->selection().indexes();
        foreach(QModelIndex index, indexes) {
            top    = qMin(top,    index.row());
            bottom = qMax(bottom, index.row());
            left   = qMin(left,   index.column());
            right  = qMax(right,  index.column());
        }

        current = d->model->index(top,left); //and paste to this index
    }

    int trPos = -1;
    int trEndPos = -1;
    int dataPos = 0;

    int top    = current.row();
    int bottom = current.row();
    int left   = current.column();
    int right  = current.column();

    if (data.indexOf("<tr", dataPos) <0)
        pasteDataInRow(current, data, right);

    while ( (trPos = data.indexOf("<tr", dataPos)) >=0) {
        if ( (trEndPos = data.indexOf(">", trPos)) <0 ) break;
        QString tag = data.mid(trPos, trEndPos-trPos+1);
        int endTrPos = data.indexOf("</tr>", trEndPos);
        if (endTrPos <0) break;

        QString rowData = data.mid(trPos +tag.length(), endTrPos-trPos-tag.length());
        QModelIndex currentInRow = current;
        right = current.column();

        pasteDataInRow(currentInRow, rowData, right);

        dataPos = endTrPos+4;
        if (data.indexOf("<tr", dataPos) >=0) {
            bottom++;
            int nextRow = current.row()+1;
            if (nextRow >= d->model->rowCount()) {
                d->model->addNewEntry();
            }
            current = d->model->index(nextRow, current.column());
        }
    }

    selectionModel()->clear();
    QModelIndex topLeft = d->model->index(top, left);
    QModelIndex bottomRight = d->model->index(bottom, right);
    selectionModel()->select(QItemSelection(topLeft, bottomRight), QItemSelectionModel::Select);

    //if (top == 0 && left == 0)
    //    d->model->parseFirstRowAsVariables();
}

void QAEditor::pasteDataInRow(QModelIndex currentInRow, QString rowData, int & right)
{
    int tdPos = -1;
    int tdEndPos = -1;
    int rowDataPos = 0;
    while ( (tdPos = rowData.indexOf("<td", rowDataPos)) >=0) {
        if ( (tdEndPos = rowData.indexOf(">", tdPos)) <0 ) break;
        QString tag = rowData.mid(tdPos, tdEndPos-tdPos+1);
        int endTdPos = rowData.indexOf("</td>", tdEndPos);
        if (endTdPos <0) break;

        QString tdData = rowData.mid(tdPos +tag.length(), endTdPos-tdPos-tag.length());
        d->model->setData(currentInRow, tdData);

        rowDataPos = endTdPos+4;
        if (rowData.indexOf("<td", rowDataPos) >=0) {
            right++;
            int nextColumn = currentInRow.column()+1;
            //if (nextColumn >= d->model->columnCount()) d->model->addColumn();
            currentInRow = d->model->index(currentInRow.row(), nextColumn);
        }
    }
}

void QAEditor::pastePlainData(QString text)
{
    if (text.isEmpty()) return; //do nothing

    QModelIndex current = currentIndex();
    if (!current.isValid())
        current = d->model->index(1,0);

    if (selectionModel()->hasSelection()) {
        int top = d->model->rowCount();
        int bottom = -1;
        int left = d->model->columnCount();
        int right = -1;

        QList<QModelIndex> indexes = selectionModel()->selection().indexes();
        foreach(QModelIndex index, indexes) {
            top    = qMin(top,    index.row());
            bottom = qMax(bottom, index.row());
            left   = qMin(left,   index.column());
            right  = qMax(right,  index.column());
        }

        current = d->model->index(top,left); //and paste to this index
    }

    int top    = current.row();
    int bottom = current.row();
    int left   = current.column();
    int right  = current.column();

    QStringList lines = text.split(QRegExp("[\n\r]"), QString::SkipEmptyParts);
    for (int i=0;i<lines.count();i++) {
        QString line = lines[i];

        QModelIndex currentInRow = current;
        right = current.column();

        QStringList args = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        for (int j=0;j<args.count();j++) {
            d->model->setData(currentInRow, args[j]);

            if (j+1 <args.count()) { //not last
                right++;
                int nextColumn = currentInRow.column()+1;
                //if (nextColumn >= d->model->columnCount()) d->model->addColumn();
                currentInRow = d->model->index(currentInRow.row(), nextColumn);
            }
        }

        if (i+1 <lines.count()) { //not last
            bottom++;
            int nextRow = current.row()+1;
            if (nextRow >= d->model->rowCount()) d->model->addNewEntry();
            current = d->model->index(nextRow, current.column());
        }
    }

    selectionModel()->clear();
    QModelIndex topLeft = d->model->index(top, left);
    QModelIndex bottomRight = d->model->index(bottom, right);
    selectionModel()->select(QItemSelection(topLeft, bottomRight), QItemSelectionModel::Select);

    //if (top == 0 && left == 0)
    //    d->model->parseFirstRowAsVariables();
}

void QAEditor::onDoubleClick(QModelIndex mi)
{
    if (mi.row() >=1) {
        edit(mi);
        return;
    }

    d->in_double_click = true;
}

void QAEditor::mouseReleaseEvent(QMouseEvent * me)
{
    QTableView::mouseReleaseEvent(me);

    if (d->in_double_click) {
        d->in_double_click = false;
        edit(currentIndex());
    }
}

