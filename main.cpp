#include <QApplication>
#include "file.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    Dialog open;


    open.show();
    return application.exec();
}
