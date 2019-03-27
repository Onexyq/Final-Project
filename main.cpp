#include <QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.setWindowTitle("Inflating Ball");
    w.resize(1080,720);

    w.show();

    return a.exec();
}
