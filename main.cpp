#include "interfacedojogo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InterfaceDoJogo w;
    w.show();

    return a.exec();
}
