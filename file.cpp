#include "file.h"
#include "ui_file.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    userInterface(new Ui::Dialog)

{
    userInterface->setupUi(this);

    QString path = QDir::homePath();

    folder = new QFileSystemModel(this);
    folder->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    folder->setRootPath(path);
    folder->setReadOnly(false);

    userInterface->treeView->setModel(folder);
    userInterface->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(userInterface->treeView,
            SIGNAL(customContextMenuRequested(const QPoint &)),
            this,
            SLOT(ContextFolderMenu(const QPoint &)));

    connect(userInterface->treeView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,
            SLOT(Show(const QModelIndex)));

    file  = new QFileSystemModel(this);
    file->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    file->setRootPath(path);
    file->setReadOnly(false);

    userInterface->listView->setModel(file);
    userInterface->listView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(userInterface->listView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(ContextFileMenu(const QPoint &)));
}

Dialog::~Dialog()
{
    delete userInterface;
}

void Dialog::Update(const QModelIndex &index)
{
    QDir::Filters TheFilter = file->filter();
    QString path = folder->fileInfo(index).absoluteFilePath();

    userInterface->listView->setRootIndex(file->setRootPath(path));
    file->setFilter(QDir::Hidden);
    file->setFilter(TheFilter);
}

//Recursively
static bool Copy(const QString &sourceFilePath, const QString &targetFilePath)
{
    QFileInfo sourceFileInfo(sourceFilePath);
    if (sourceFileInfo.isDir())
    {
        QDir targetFolder(targetFilePath);
     targetFolder.cdUp();
     if (!targetFolder.mkdir(QFileInfo(targetFilePath).fileName()))
         return false;
     QDir sourceFolder(sourceFilePath);
     QStringList fileNames = sourceFolder.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

     foreach (const QString &filename, fileNames)
     {
      const QString newSourceFilePath = sourceFilePath + QLatin1Char('/') + filename;
      const QString newTargetFilePath = targetFilePath + QLatin1Char('/') + filename;
      if (!Copy(newSourceFilePath,newTargetFilePath))
         return false;
     }
    } else
    {
        if(!QFile::copy(sourceFilePath,targetFilePath))
            return false;
    }
    return true;
}

void Dialog::ContextFolderMenu(const QPoint &pos)
{
    QModelIndex index = userInterface->treeView->currentIndex();
    QString startPath = folder->filePath(index);
    QPoint item = userInterface->treeView->mapToGlobal(pos);
    QMenu menu;

    menu.addAction("Delete");
    menu.addAction("Copy");
    menu.addAction("Create new Folder");

    QAction* rightClickItem = menu.exec(item);

    if (rightClickItem && rightClickItem->text().contains("Delete"))
    folder->remove(index);


    if (rightClickItem && rightClickItem->text().contains("Copy"))
    {
        QString destination  =  QFileDialog::getExistingDirectory(this,tr("Open the Folder"),
        QDir::homePath(),QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks).append("/").append(folder->fileName(index));
     Copy(startPath,destination);
    };


    if (rightClickItem && rightClickItem->text().contains("Make new Folder"))
    {
        QModelIndex index = userInterface->treeView->currentIndex();
        if (!index.isValid()) return;

        QString name = QInputDialog::getText(this,"Name","Enter the Folder name");
        if (name.isEmpty()) return;
        folder->mkdir(index,name);
    };
}

void Dialog::ContextFileMenu(const QPoint &pos)
{
    QModelIndex index = userInterface->listView->currentIndex();
    QString startpath = file->filePath(index);
    QPoint item = userInterface->listView->mapToGlobal(pos);
    QMenu menu;

    menu.addAction("Delete");
    menu.addAction("Copy");

    QAction* rightClickItem = menu.exec(item);

    if (rightClickItem && rightClickItem->text().contains("Delete"))
    file->remove(index);

    if (rightClickItem && rightClickItem->text().contains("Copy"))
    {
        QString destination = QFileDialog::getExistingDirectory(this,tr("Open the Folder name"),
        QDir::homePath(),QFileDialog::ShowDirsOnly
      | QFileDialog::DontResolveSymlinks).append("/").append(QInputDialog::getText(this,"Name","Enter a new name"));

        QFile::copy(startpath,destination);
    };
}



