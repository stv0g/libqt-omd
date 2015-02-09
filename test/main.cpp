#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>

#include "viewer.h"
#include "camera.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSystemTrayIcon tray(&app);
    QMenu menu;

    Viewer viewer;
    Oi::Camera cam;

    /* Actions */
    QAction showLiveView("Show Live View", &tray);
    QObject::connect(&showLiveView, &QAction::triggered, [&]() {
        viewer.show();
    });

    QAction openBrowser("Open Browser", &tray);
    QObject::connect(&openBrowser, &QAction::triggered, [&]() {
        QDesktopServices::openUrl(QUrl("http://192.168.0.10"));
    });

    QAction closeApp("Quit", &tray);
    QObject::connect(&closeApp, &QAction::triggered, [&]() {
        app.quit();
    });

    /* Camera Signals */
    QAction sizeInfo("Capacity: ?", &tray);
    QObject::connect(&cam, &Oi::Camera::capacityUpdated, [&](int cap) {
        sizeInfo.setText(QString("Capacity: %1").arg(cap));
        tray.setContextMenu(&menu);
    });

    /* Context Menu */
    menu.addAction(&showLiveView);
    menu.addAction(&openBrowser);
    menu.addSeparator();
    menu.addAction(&sizeInfo);
    menu.addSeparator();
    menu.addAction(&closeApp);

    QIcon icon(":/icon/libqt-omd_bw.png");
    tray.setIcon(icon);
    tray.setContextMenu(&menu);
    tray.show();

    return app.exec();
}
