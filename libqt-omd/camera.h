/** Oi Camera Class
 *
 * @copyright	2015 Steffen Vogel
 * @license     http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author      Steffen Vogel <post@steffenvogel.de>
 * @link        https://dev.0l.de/wiki/projects/omd/start
 * @package     libqt-omd
 */
/*
 * This file is part of libqt-omd
 *
 * libqt-omd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libqt-omd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libqt-omd. If not, see <http://www.gnu.org/licenses/>.
 */

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
    QUrl                    getUrl() const;

    Oi::LiveView *          startLiveView(QSize resolution = QSize(640, 480), int port = 22222);
    void                    stopLiveView();

    /* Cached getters */
    bool                    isOnline();
    enum CamMode            camMode();

public slots:
    void                    initialize();
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
