//#ifndef DIALOG_H
//#define DIALOG_H
#define FILE_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QMenu>
#include <QItemSelectionModel>
#include <QLine>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QDir>
#include <QMainWindow>
#include <QDirModel>
#include <QInputDialog>
#include <QDesktopWidget>



namespace Ui
{
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void Update(const QModelIndex &index);

    void ContextFolderMenu(const QPoint &);

    void ContextFileMenu(const QPoint &);

private:
    Ui::Dialog *userInterface;
    QFileSystemModel *folder;
    QFileSystemModel *file;
    QDirModel *datamodel;
};

//#endif // DIALOG_H
