#ifndef VIEWER_H
#define VIEWER_H

#include <QImage>
#include <QGLWidget>

class Viewer : public QGLWidget
{
public:
    Viewer();

public slot:
    newFrame(QImage *);

protected:
    paintEvent(QPaintEvent *);

    QImage lastImage;
};

#endif // VIEWER_H
