#include "MIS_Kryuchkov.h"
#include <QtWidgets/qapplication.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MIS_Kryuchkov w;
    w.grabGesture(Qt::PanGesture);
    w.grabGesture(Qt::PinchGesture);
    w.show();
    return a.exec();
}
