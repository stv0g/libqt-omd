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

    enum ConnectMode {
        CONNECT_PRIVATE,
        CONNECT_SHARED
    };

    QList<QString> getImageList(bool rsv = false);
    QImage         getLastImage();
    QImage         getImage(QString name, QSize resolution = QSize(-1, -1));
    QImage         getThumbnail(QString name);


    bool isOnline();

    void setProperty(QString key, QString value);
    int getCapacity();

    void setTakeMode(enum TakeMode m);


public slots:
    void takeShot();
    void powerOff();

    LiveView startLiveView(QSize resolution = QSize(640, 480));

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

    void      setMode(enum Mode mode);
    enum Mode getMode();

    void getCommandList();

    /* Parsers */
    void parseCommandList(QNetworkReply *reply);
    void parseProperties(QNetworkReply *reply);
    void parseImageList(QNetworkReply *reply);
    void parseCapacity(QNetworkReply *reply);
    void parseConnectMode(QNetworkReply *reply);

    QNetworkAccessManager networkManager;
    QHostAddress address;

    LiveView liveView;

    QList<QSize> liveViewResolutions;
    QList<QSize> imgResolutions;
};

#endif // CAMERA_H
