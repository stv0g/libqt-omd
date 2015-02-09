#include <QtDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QTcpSocket>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFileInfo>

#include "image.h"
#include "properties.h"
#include "camera.h"

using namespace Oi;

const QString Camera::cUserAgent = "libqt-omd v0.1";

Camera::Camera() :
    mAddress("192.168.0.10"),
    mCamMode(MODE_UNKNOWN),
    mConnectMode(CONNECT_UNKNOWN)
{
    connect(&mNetworkManager, &QNetworkAccessManager::finished, this, &Camera::requestFinished);

    initialize();
}

void Camera::initialize()
{
    switchCamMode(MODE_PLAY);
    //completePendingRequests();

    requestCamInfo();
    requestCapacity();
    requestConnectMode();
    requestCommandList();

    requestImageList();

    completePendingRequests();
}

bool Camera::isOnline()
{
    QTcpSocket socket;
    socket.connectToHost(camAddress, 80, QIODevice::ReadOnly);
    return socket.waitForConnected(2000);
}

/********* Request handling ***********/

QNetworkRequest Camera::makeRequest(QString cgi, QMap<QString, QString> params)
{
    QString tpl = QString("http://%1/%2.cgi").arg(camAddress.toString()).arg(cgi);

    QUrl url(tpl);
    QUrlQuery query(url);
    QList<QPair<QString, QString>> paramList;

    for (QString key : params.keys())
        paramList.push_back(qMakePair(key, params[key]));

    query.setQueryItems(paramList);
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, userAgent);

    return request;
}

QNetworkReply * Camera::get(QString cgi, QMap<QString, QString> params)
{
    QNetworkRequest request = makeRequest(cgi, params);
    QNetworkReply *reply = networkManager->get(request);

    qDebug() << "[libqt-omd] New GET request:" << request.url();

    pendingReplies.append(reply);
    return reply;
}

QNetworkReply * Camera::post(QString cgi, QMap<QString, QString> params, QDomDocument body)
{
    QNetworkRequest request = makeRequest(cgi, params);
    QNetworkReply *reply = networkManager->post(request, body.toByteArray());

    qDebug() << "[libqt-omd] New POST request:" << request.url();

    pendingReplies.append(reply);
    return reply;
}

void Camera::completePendingRequests()
{
    static QEventLoop loop;

    connect(networkManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::wakeUp);

    while (!pendingReplies.empty()) {
        qDebug() << "[libqt-omd] Entering event loop!";
        loop.exec();
    }
}

void Camera::requestFinished(QNetworkReply *reply)
{
    QDomDocument xml;

    qDebug() << "[libqt-omd] Request finished:" << reply->url();

    if (reply->error() == QNetworkReply::NoError) {
        QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        QString cgi         = reply->attribute(QNetworkRequest::User).toString();

        cacheMap[cgi] = QDateTime::currentDateTimeUtc();

        if (contentType == "text/xml"         && reply->size() > 0) {
            QDomDocument body;
            if (body.setContent(reply->readAll())) {
                qDebug() << "[libqt-omd]   Content:" <<  body.toString();
                parseXml(cgi, body);
            }
            else
                qCritical() << "Failed to parse XML reply";
        }
        else if (contentType == "text/plain"  && reply->size() > 0)
            parseList(cgi, reply->readAll());
        else if (contentType == "www/unknown" && reply->size() == 0)
            parseEmpty(cgi);
        else if (contentType == "image/jpeg")
            parseImage(cgi, reply->readAll());
        else
            qCritical() << "Failed to parse reply: Content-Type = " << contentType
                        << ", Size = " << reply->size()
                        << ", URI  = " << reply->request().url();
    }
    else
       qWarning() << "Request failed: " << reply->errorString();

    pendingReplies.removeAll(reply);
}

/********* Actions ***********/

void Camera::controlZoom(ZoomMode cmd)
{
    Q_UNUSED(cmd)
    // FIXME implement
}

void Camera::setTimeDiff(QDateTime t)
{
    Q_UNUSED(t)
    // FIXME implement
}

void Camera::reFocus(QPoint pos, QSize size)
{
    Q_UNUSED(pos)
    Q_UNUSED(size)
    // FIXME implement
}

/********* Requests **********/

void Camera::takeShot()                 { get("exec_takemotion"); }
void Camera::powerOff()                 { get("exec_pwoff"); }

void Camera::requestCamInfo()           { get("get_caminfo"); }
void Camera::requestCapacity()          { get("get_unusedcapacity"); }
void Camera::requestConnectMode()       { get("get_connectmode"); }
void Camera::requestCommandList()       { get("get_commandlist"); }

void Camera::requestImageList(QString dir, bool rsv) {
    QMap<QString, QString> params;

    params["DIR"] = dir.replace('/', "%2F");

    get(rsv ? "get_rsvimglist" : "get_imglist", params);
}

void Camera::requestImage(QString name, QSize resolution)
{
    QString tpl = QString("http://%1/DCIM/100OLYMP/%1.JPG").arg(camAddress.toString()).arg(name);
    Q_UNUSED(resolution)

    QUrl url(tpl);
    QNetworkRequest request(url);

    networkManager->get(request);
}

void Camera::switchCamMode(CamMode mode)
{
    QMap<QString, QString> params;

    switch (mode) {
        case MODE_PLAY:
            params["mode"] = "play";
            break;
        case MODE_RECORD:
            params["mode"] = "rec";
            params["lvqty"] = "0640x0480"; // FIXME allow other liveViewQualities
            break;
        case MODE_SHUTTER:
            params["mode"] = "shutter";
            break;
    }

    get("switch_cammode", params);
}

/*********** Reply parsers ************/

void Camera::parseXml(QString cgi, QDomDocument body)
{
    if      (cgi == "get_unusedcapacity") parseCapacity(body);
    else if (cgi == "get_commandlist")    parseCommandList(body);
    else if (cgi == "get_camprop")        parseProperties(body);
    else if (cgi == "set_camprop")        parseProperties(body);
    else if (cgi == "get_connectmode")    parseConnectMode(body);
    else if (cgi == "exec_takemotion")    parseTakeMotion(body);
    else if (cgi == "exec_takemisc")      parseTakeMisc(body);
}

void Camera::parseList(QString cgi, QByteArray body)
{
    bool mark = (cgi == "get_rsvimglist");

    for (QByteArray line : body.split('\n')) {
        OiImage img(line, mark, this);

        images[img.path()] = img;
    }
}

void Camera::parseImage(QString cgi, QByteArray body)
{
    Q_UNUSED(cgi)

    QImage img = QImage::fromData(body.data());

    emit receivedImage(img);
}

void Camera::parseEmpty(QString cgi, QNetworkReply *reply)
{
    // FIXME anything to do here?
}

void Camera::parseCamInfo(QDomDocument body)
{
    QDomElement elm = body.firstChildElement("caminfo");
    if (!elm.isNull()) {
        QDomElement model = elm.firstChildElement("model");
        camModel = model.text();
    }
}

void Camera::parseCapacity(QDomDocument body)
{
    QDomElement elm = body.firstChildElement("unused");
    if (!elm.isNull())
        unusedCapacity = elm.text().toInt();
}

void Camera::parseCommandList(QDomDocument body)
{
    commandList = body;
}

void Camera::parseConnectMode(QDomDocument body)
{
    QDomElement elm = body.firstChildElement("connectmode");
    if (!elm.isNull()) {
        if (elm.text() == "private")
            connectMode = CONNECT_PRIVATE;
        else if (elm.text() == "shared")
            connectMode = CONNECT_SHARED;
        else
            qWarning() << "[libqt-omd] Warning: unknown connectMode:" << elm.text();
    }

}

void Camera::parseProperties(QDomDocument body)
{
    Q_UNUSED(body)
    // FIXME implement
}

void Camera::parseTakeMisc(QDomDocument body)
{
    Q_UNUSED(body)
    // FIXME implement
}

void Camera::parseTakeMotion(QDomDocument body)
{
    Q_UNUSED(body)
    // FIXME implement
}
