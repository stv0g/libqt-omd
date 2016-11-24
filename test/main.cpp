#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

#include "mainwindow.h"
#include "camera.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSystemTrayIcon tray(&app);
    QMenu menu;

    Oi::Camera cam;
    MainWindow win(&cam);

    /* Actions */
    QAction openWindow("Open", &tray);
    QAction closeApp("Quit", &tray);
    QAction openBrowser("Open Browser", &tray);

    QObject::connect(&closeApp,   &QAction::triggered, &win, &QMainWindow::close);
    QObject::connect(&openWindow, &QAction::triggered, &win, &QMainWindow::show);

    QObject::connect(&openBrowser, &QAction::triggered, [&]() {
        QDesktopServices::openUrl(cam.url());
    });

    /* Context Menu */
    menu.addAction(&openWindow);
    menu.addAction(&openBrowser);
    menu.addSeparator();
    menu.addAction(&closeApp);

    QIcon icon(":/icon/libqt-omd_gray.png");
    tray.setIcon(icon);
    tray.setContextMenu(&menu);
    tray.show();

    /* Initialize camera */
    if (cam.isOnline())
        cam.initialize();
    else
        QMessageBox::critical(&win, "Error", "Failed to connect to camera!");

    return app.exec();
}
