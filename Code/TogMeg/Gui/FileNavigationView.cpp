
#include <QtGui>
#include "FileNavigationView.h"
#include "FileNavigationModel.h"

#include "AppStyles.h"

class FileNavigationView::Private {
public:
    QPointer<FileNavigationModel> model;
    QModelIndex prevUnderMouse;
};

FileNavigationView::FileNavigationView(QWidget * parent)
:QTreeView(parent)
{
    d = new Private;
    
    setHeaderHidden(true);
    setMouseTracking(true);
    setRootIsDecorated(false);
    setUniformRowHeights(true);
    setAllColumnsShowFocus(true);
    setAlternatingRowColors(false);
    setContentsMargins(0,5,0,0);
    setEditTriggers(QAbstractItemView::SelectedClicked | 
                    QAbstractItemView::AnyKeyPressed);
    setAttribute(Qt::WA_MacShowFocusRect, false);
#ifdef Q_WS_MAC
    setFrameStyle(QFrame::NoFrame);
    setContentsMargins(1,60,1,1);
    setLineWidth(0);
#endif

    header()->setStretchLastSection(false);
    
    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(214,221,229));
    setAutoFillBackground(true);
    setPalette(p);
}

FileNavigationView::~FileNavigationView()
{
    delete d;
}

void FileNavigationView::setModel(FileNavigationModel * m)
{
    d->model = m;
    QTreeView::setModel(m);
    header()->setResizeMode(FileNavigationModel::ColName, QHeaderView::Stretch);

    connect(m, SIGNAL(aboutToLoad()), this, SLOT(m_aboutToLoad()));
    connect(m, SIGNAL(loaded()), this, SLOT(m_loaded()));
    connect(this, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(activateItem(const QModelIndex &)));
    connect(selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(selectItem(const QModelIndex &)));
}

void FileNavigationView::showEvent(QShowEvent * se)
{
    if (!se->spontaneous()) {
        if (!d->model) {
            setModel(new FileNavigationModel(this));
        }
    }
    QTreeView::showEvent(se);
}

void FileNavigationView::keyReleaseEvent(QKeyEvent * ke)
{
    if (ke->key() != Qt::Key_Return)
        return QTreeView::keyReleaseEvent(ke);
    
    QModelIndex i = currentIndex();

    if (!i.isValid()) return QTreeView::keyReleaseEvent(ke);
    if (!model()) return QTreeView::keyReleaseEvent(ke);
    
    FileNavigationModel * m = dynamic_cast<FileNavigationModel *>(model());
    if (!m) return QTreeView::keyReleaseEvent(ke);
    
    activateItem(i);
    ke->accept();
}

void FileNavigationView::addItem()
{
    FileNavigationModel * m = dynamic_cast<FileNavigationModel *>(model());
    if (!m) return;

    QString path = m->path();
    QDir dir(path);

    uint i=0;
    QString folder;
    do {
        i++;
        folder = tr("Group %1").arg(i,3,10,QChar('0'));
    }
    while(dir.exists(folder));

    dir.mkdir(folder);
    m->reload();

    for (int row=0; row< m->rowCount(); row++) {
        QModelIndex mi = m->index(row,0);
        QString f = mi.data().toString();
        if (f==folder) {
            clearSelection();
            setCurrentIndex(mi);
            break;
        }
    }
}

void FileNavigationView::removeItem()
{

}

void FileNavigationView::selectItem(const QModelIndex & i)
{
    if (!i.isValid()) return;
    if (!model()) return;

    FileNavigationModel * m = dynamic_cast<FileNavigationModel *>(model());
    if (!m) return;

    QFileInfo * fi = m->fileInfo(i);
    if (!fi->isDir())
        emit openFileRequest(fi->absoluteFilePath());
}

void FileNavigationView::activateItem(const QModelIndex & i)
{
    if (!i.isValid()) return;
    if (!model()) return;
    
    FileNavigationModel * m = dynamic_cast<FileNavigationModel *>(model());
    if (!m) return;
    
    QFileInfo * fi = m->fileInfo(i);
    if (fi->isDir()) {
        QString fileName = i.data().toString();
        return m->loadPathContent( m->path()+fileName );
    }
}

void FileNavigationView::resizeEvent(QResizeEvent * re)
{
    QTreeView::resizeEvent(re);
    emit widthChanged(re->size().width());
}

void FileNavigationView::m_aboutToLoad()
{
    setCurrentIndex(QModelIndex());
}

void FileNavigationView::m_loaded()
{
    if (model()->rowCount())
        setCurrentIndex(model()->index(0,0));
}

