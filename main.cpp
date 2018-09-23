#include "SingalView.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SingalView w;
    w.show();

    return a.exec();
}
