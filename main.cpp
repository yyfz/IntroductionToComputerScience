#include "widget.h"

#include <QApplication>

bool Widget::isShowingName = false;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
