#include <QApplication>

#include "widget.h"
#include "appkey.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //������Կ������
    //AppKey::Instance()->start();

    Widget w;
    w.show();
   
    return a.exec();
}
