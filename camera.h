#ifndef CAMERA_H
#define CAMERA_H

#include <QPair>
#include <QString>
#include <QImage>
#include <QNetworkAccessManager>
#include <QHostAddress>
#include <QDateTime>
#include <QtXml/QDomDocument>

#include "liveview.h"

class Camera
{
    Q_OBJECT
    // FIXME add Q_PROPERTY

public:
    /* Enums */
    enum CamMode {
        MODE_RECORD,
        MODE_PLAY,
        MODE_SHUTTER
    };

    enum ConnectMode {
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

    void                setProperty(QString key, QString value);
    QString             getProperty(QString key);

    /* Asynchronous requests */;
    void                requestCamInfo();
    void                requestCapacity();
    void                requestConnectMode();
    void                requestCommandList();
    void                requestImageList(bool reserved = false);
    void                requestImage(QString name = QString(), QSize resolution = QSize(-1, -1));

    /* Synchronous getters */
    QString             getCamInfo();
    int                 getCapacity();
    enum ConnectMode    getConnectMode();
    void /* FIXME */    getCommandList();
    QList<QString>      getImageList(bool reserved = false);
    QImage              getImage(QString name = QString(), QSize resolution = QSize());

    LiveView *          startLiveView(QSize resolution = QSize(640, 480), int port = 22222);
    void                stopLiveView();

    /* Cached getters */
    bool                isOnline();
    enum Mode           getCamMode();

public slots:
    void                switchCamMode(enum CamMode mode);
    void                controlZoom(enum ZoomMode cmd);
    void                takeShot();
    void                reFocus(QPoint pos, QSize size);
    void                powerOff();
    void                setTimeDiff(QDateTime t);

signals:
    void                receivedImage(QImage *);
    void                changedProperty(QString key, QString value);
    void                changedMode(enum Mode);
    void                connected(enum ConnectMode);

protected slots:
    void                requestFinished(QNetworkReply *);

protected:
    /* Network requests */
    QNetworkRequest     makeRequest(QString cgi, QPair<QString, QString> params);
    QNetworkReply               get(QString cgi, QPair<QString, QString> params = QPair<QString, QString>());
    QNetworkReply              post(QString cgi, QPair<QString, QString> params = QPair<QString, QString>(), QDomDocument body);

    /* Parsers */
    void                parseCamInfo(QDomDocument *body);
    void                parseCapacity(QDomDocument *body);
    void                parseConnectMode(QDomDocument *body);
    void                parseCommandList(QDomDocument *body);
    void                parseCamProperties(QDomDocument *body);
    void                parseImage(QByteArray *body);

    /* Helpers */
    void                completeRequests();

    /* Members */
    QHostAddress             camAddress;
    QList<QNetworkReply *>   pendingReplies;
    QMap<QString, QDateTime> cacheMap;
    QNetworkAccessManager   *networkManager;
    LiveView                *liveView;

    /* Properties */
    enum CamMode        camMode;
    enum ConnectMode    connectMode;

    QString             camModel;
    int                 unusedCapacity;

    QDomDocument        commandList;
    QList<QSize>        liveViewResolutions;
    QList<QSize>        resizeResolutions;

    QList<QString>      imageList;
    QList<QString>      reservedImageList;

    /* Constants */
    static const QString userAgent = "OI.Share v2";
};

#endif // CAMERA_H
