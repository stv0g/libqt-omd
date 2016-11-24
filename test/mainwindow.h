#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <image.h>
#include <camera.h>
#include <imagelistmodel.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Oi::Camera *c, QWidget *parent = 0);
    ~MainWindow();

protected:
    void updateProperties(Oi::Properties *props);

    Ui::MainWindow *ui;
    Oi::Camera *cam;
    Oi::ImageListModel imageListModel;

    QMap<QString, QPair<QString, QString>> propInfo;
};

#endif // MAINWINDOW_H
