#include "MIS_Kryuchkov.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MIS_Kryuchkov w;
    w.show();
    return a.exec();
}
