#include <QApplication>

#include "viewer.h"
#include "camera.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Viewer w;
    OiCamera c;

    w.show();

    return a.exec();
}
