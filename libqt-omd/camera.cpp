#include <QtDebug>
#include <QUrl>
#include <QUrlQuery>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>

#include "image.h"
#include "properties.h"
#include "camera.h"

const QString OiCamera::userAgent = "libqt-omd v0.1";

OiCamera::OiCamera() :
    camAddress("192.168.0.10")
{
    networkManager = new QNetworkAccessManager;

    connect(networkManager, &QNetworkAccessManager::finished, this, &OiCamera::requestFinished);

    initialize();
}

void OiCamera::initialize()
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

bool OiCamera::isOnline()
{
    QTcpSocket socket;
    socket.connectToHost(camAddress, 80, QIODevice::ReadOnly);
    return socket.waitForConnected(2000);
}

/********* Request handling ***********/

QNetworkRequest OiCamera::makeRequest(QString cgi, QMap<QString, QString> params)
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

QNetworkReply * OiCamera::get(QString cgi, QMap<QString, QString> params)
{
    QNetworkRequest request = makeRequest(cgi, params);
    QNetworkReply *reply = networkManager->get(request);

    qDebug() << "[libqt-omd] New GET request:" << request.url();

    pendingReplies.append(reply);
    return reply;
}

QNetworkReply * OiCamera::post(QString cgi, QMap<QString, QString> params, QDomDocument body)
{
    QNetworkRequest request = makeRequest(cgi, params);
    QNetworkReply *reply = networkManager->post(request, body.toByteArray());

    qDebug() << "[libqt-omd] New POST request:" << request.url();

    pendingReplies.append(reply);
    return reply;
}

void OiCamera::completePendingRequests()
{
    static QEventLoop loop;

    connect(networkManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::wakeUp);

    while (!pendingReplies.empty()) {
        qDebug() << "[libqt-omd] Entering event loop!";
        loop.exec();
    }
}

void OiCamera::requestFinished(QNetworkReply *reply)
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

void OiCamera::controlZoom(ZoomMode cmd)
{
    // FIXME implement
}

void OiCamera::setTimeDiff(QDateTime t)
{
    // FIXME implement
}

void OiCamera::reFocus(QPoint pos, QSize size)
{
    // FIXME implement
}

/********* Requests **********/

void OiCamera::takeShot()                 { get("exec_takemotion"); }
void OiCamera::powerOff()                 { get("exec_pwoff"); }

void OiCamera::requestCamInfo()           { get("get_caminfo"); }
void OiCamera::requestCapacity()          { get("get_unusedcapacity"); }
void OiCamera::requestConnectMode()       { get("get_connectmode"); }
void OiCamera::requestCommandList()       { get("get_commandlist"); }

void OiCamera::requestImageList(QString dir, bool rsv) {
    QMap<QString, QString> params;

    params["DIR"] = dir.replace('/', "%2F");

    get(rsv ? "get_rsvimglist" : "get_imglist", params);
}

void OiCamera::requestImage(QString name, QSize resolution)
{
    QString tpl = QString("http://%1/DCIM/100OLYMP/%1.JPG").arg(camAddress.toString()).arg(name);
    QUrl url(tpl);
    QNetworkRequest request(url);

    networkManager->get(request);
}

void OiCamera::switchCamMode(CamMode mode)
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

void OiCamera::parseXml(QString cgi, QDomDocument body)
{
    if      (cgi == "get_unusedcapacity") parseCapacity(body);
    else if (cgi == "get_commandlist")    parseCommandList(body);
    else if (cgi == "get_camprop")        parseProperties(body);
    else if (cgi == "set_camprop")        parseProperties(body);
    else if (cgi == "get_connectmode")    parseConnectMode(body);
    else if (cgi == "exec_takemotion")    parseTakeMotion(body);
    else if (cgi == "exec_takemisc")      parseTakeMisc(body);
}

void OiCamera::parseList(QString cgi, QByteArray body)
{
    bool mark = (cgi == "get_rsvimglist");

    for (QByteArray line : body.split('\n')) {
        OiImage img(line, mark, this);

        images[img.path()] = img;
    }
}

void OiCamera::parseImage(QString cgi, QByteArray body)
{
    QImage img = QImage::fromData(body.data());

    emit receivedImage(img);
}

void OiCamera::parseEmpty(QString cgi)
{
    // FIXME anything to do here?
}

void OiCamera::parseCamInfo(QDomDocument body)
{
    QDomElement elm = body.firstChildElement("caminfo");
    if (!elm.isNull()) {
        QDomElement model = elm.firstChildElement("model");
        camModel = model.text();
    }
}

void OiCamera::parseCapacity(QDomDocument body)
{
    QDomElement elm = body.firstChildElement("unused");
    if (!elm.isNull())
        unusedCapacity = elm.text().toInt();
}

void OiCamera::parseCommandList(QDomDocument body)
{
    commandList = body;
}

void OiCamera::parseConnectMode(QDomDocument body)
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

void OiCamera::parseProperties(QDomDocument body)
{

}

void OiCamera::parseTakeMisc(QDomDocument body)
{

}

void OiCamera::parseTakeMotion(QDomDocument body)
{

}
