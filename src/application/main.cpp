#include <QApplication>
#include <QMainWindow>

#include "core.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    qApp->setApplicationVersion("1.0.1");
    QMainWindow* vpWindow = getWindow();
    if (vpWindow)
    {
        vpWindow->show();
    }
    else
    {
        return -1;
    }
    int vRes = a.exec();
    if (vpWindow) delete vpWindow;
    return vRes;
}
