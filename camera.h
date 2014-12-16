#ifndef CAMERA_H
#define CAMERA_H

#include <QPair>
#include <QString>
#include <QImage>
#include <QNetworkAccessManager>
#include <QHostAddress>


#include <QtXml/QDomDocument>

#include "liveview.h"

class Camera
{
public:
    Camera();

    enum Mode {
        MODE_RECORD,
        MODE_PLAY,
        MODE_SHUTTER
    };

    enum TakeMode {
        PROG_IAUTO,
        PROG_P,
        PROG_A,
        PROG_S,
        PROG_M,
        PROG_ART
    };

    QList<QString> getImageList(bool rsv = false);
    QImage         getImage(QString name, QSize resolution = QSize(-1, -1));


    bool isOnline();

    void setProperty(QString key, QString value);
    int getCapacity();

public slots:
    void takeShot();
    void powerOff();

    void startLiveView(QSize resolution = QSize(640, 480));
    void stopLiveView();

protected slots:
    void requestFinished(QNetworkReply *);

signals:
    void imageReceived(QImage *img);
    void newFrame(QImage *img);

    void capacityUpdated(int);
    void propertyReceived(QString key, QString value);

protected:
    QNetworkRequest makeRequest(QString cgi, QPair<QString, QString> params);
    QNetworkReply  get(QString cgi, QPair<QString, QString> params = QPair<QString, QString>());
    QNetworkReply post(QString cgi, QPair<QString, QString> params = QPair<QString, QString>(), QDomDocument body);

    int       setMode(enum Mode mode);
    enum Mode getMode();

    QNetworkAccessManager networkManager;
    QHostAddress address;

    LiveView liveView;

    QList<QSize> liveViewResolutions;
    QList<QSize> imgResolutions;
};

#endif // CAMERA_H
