#ifndef CAMERA_H
#define CAMERA_H

#include <QHostAddress>
#include <QImage>
#include <QDateTime>
#include <QtXml/QDomDocument>
#include <QNetworkAccessManager>
#include <QEventLoop>

#include "properties.h"
#include "image.h"
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

namespace Oi {
    class LiveView;
    class Camera;
}

class Oi::Camera : public QObject
{
    Q_OBJECT
    // FIXME add Q_PROPERTY's

public:
    /* Enums */
    enum CamMode {
        MODE_UNKNOWN,
        MODE_RECORD,
        MODE_PLAY,
        MODE_SHUTTER
    };

    enum ConnectMode {
        CONNECT_UNKNOWN,
        CONNECT_PRIVATE,
        CONNECT_SHARED
    };

    enum ZoomMode {
        ZOOM_OFF,
        ZOOM_WIDEMOVE,
        ZOOM_TELEMOVE,
        ZOOM_WIDETERM,
        ZOOM_TELETERM
    };

    Camera();

    void                    setProperty(QString key, QString value);
    QString                 getProperty(QString key);

    /* Asynchronous requests */;
    void                    requestCamInfo();
    void                    requestCapacity();
    void                    requestConnectMode();
    void                    requestCommandList();
    void                    requestImageList(QString dir = "/DCIM/100OLYMP", bool reserved = false);
    void                    requestImage(QString name = QString(), QSize resolution = QSize(-1, -1));

    /* Synchronous getters */
    QString                 getCamInfo();
    unsigned                getCapacity();
    enum ConnectMode        getConnectMode();
    void /* FIXME */        getCommandList();
    QList<QString>          getImageList(bool reserved = false);
    QImage                  getImage(QString name = QString(), QSize resolution = QSize());

    Oi::LiveView *          startLiveView(QSize resolution = QSize(640, 480), int port = 22222);
    void                    stopLiveView();

    /* Cached getters */
    bool                    isOnline();
    enum CamMode            camMode();

public slots:
    void                    switchCamMode(enum CamMode mode);
    void                    controlZoom(enum ZoomMode cmd);
    void                    takeShot();
    void                    reFocus(QPoint pos, QSize size);
    void                    powerOff();
    void                    setTimeDiff(QDateTime t);

signals:
    void                    receivedImage(QImage);
    void                    capacityUpdated(long unsigned);
    void                    changedProperty(QString key, QString value);
    void                    changedMode(enum CamMode);
    void                    modelUpdated(QString);
    void                    poweredOff();
    void                    connected(enum ConnectMode);

protected slots:
    void                    requestFinished(QNetworkReply *);

protected:
    /* Network requests */
    QNetworkRequest  makeRequest(QString cgi, QMap<QString, QString> params);
    QNetworkReply *          get(QString cgi, QMap<QString, QString> params = QMap<QString, QString>());
    QNetworkReply *         post(QString cgi, QMap<QString, QString> params = QMap<QString, QString>(), QDomDocument body = QDomDocument());

    /* Parsers */
    void                    parseEmpty(QString cgi, QNetworkReply *reply = NULL);
    void                    parseXml(QString cgi, QDomDocument body);
    void                    parseList(QString cgi, QByteArray body);
    void                    parseImage(QString cgi, QByteArray body);

    void                    parseCamInfo(QDomDocument body);
    void                    parseCapacity(QDomDocument body);
    void                    parseConnectMode(QDomDocument body);
    void                    parseCommandList(QDomDocument body);
    void                    parseProperties(QDomDocument body);
    void                    parseTakeMotion(QDomDocument body);
    void                    parseTakeMisc(QDomDocument body);

    /* Helpers */
    void                    completePendingRequests();
    void                    initialize();

    /* Members */
    QHostAddress            mAddress;
    QEventLoop              mLoop;
    QList<QNetworkReply *>  mPendingReplies;
    QNetworkAccessManager   mNetworkManager;

    /* Properties */
    enum CamMode            mCamMode;
    enum ConnectMode        mConnectMode;

    QString                 mCamModel;
    long unsigned           mUnusedCapacity;

    QDomDocument            mCommandList;

    QHash<QString, Image> mImages;

    /* Constants */
    static const QString    cUserAgent;
};

#endif // CAMERA_H
