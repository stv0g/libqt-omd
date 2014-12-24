#include <QtDebug>
#include <QUrl>
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>

#include "camera.h"

Camera::Camera() :
    address("192.168.0.10")
{
    networkManager = new QNetworkAccessManager;

    connect(networkManager, &QNetworkAccessManager::finished, this, &Camera::requestFinished);

    intialize();
}

void Camera::initialize()
{
    requestCamInfo();
    requestCapacity();
    requestCommandList();
    requestImageList();

    completeRequests();
}

bool Camera::isOnline()
{
    QTcpSocket socket;
    socket.connectToHost(address, 80, QIODevice::ReadOnly);
    return socket.waitForConnected(2000);
}

/********* Request handling ***********/

QNetworkRequest Camera::makeRequest(QString cgi, QPair<QString, QString> params)
{
    QString tpl("http://%1/%2.cgi");
    tpl.arg(address.toString()).arg(cgi);

    QUrl url(tpl);
    url.setQueryItems(params);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
    request.setAttribute(QNetworkRequest::User, QVariant(cgi));
    request.setAttribute(QNetworkRequest::User+1, QVariant(params));

    return request;
}

QNetworkReply Camera::get(QString cgi, QPair<QString, QString> params)
{
    QNetworkRequest request = makeRequest(cgi, params);

    // FIXME check with commandList

    return networkManager.get(request);
}

QNetworkReply Camera::post(QString cgi, QPair<QString, QString> params, QDomDocument body)
{
    QNetworkRequest request = makeRequest(cgi, params);

    // FIXME check with commandList

    return networkManager.post(request, body.toByteArray());
}

void Camera::completeAllRequests()
{
    static QEventLoop loop;

    while (!requests.empty())
        loop.exec();
}

void Camera::requestFinished(QNetworkReply *reply)
{
    QDomDocument xml;

    if (reply->error() == QNetworkReply::NoError) {
        QString contentType = reply->header(QNetworkRequest::ContentTypeHeader);
        QString cgi         = reply->attribute(QNetworkRequest::User);

        cacheMap[cgi] = QDateTime::currentDateTimeUtc();

        if (contentType == "text/xml"         && reply->size() > 0) {
            QDomDocument body;
            if (body.setContent(reply->readAll()))
                parseXml(cgi, body);
            else
                qCritical() << "Failed to parse XML reply";
        }
        else if (contentType == "text/plain"  && reply->size() > 0)
            parseList(cgi, reply->readAll());
        else if (contentType == "www/unknown" && reply->size() == 0)
            parse(cgi);
        else if (contentType == "image/jpeg")
            parseImage(reply->readAll());
        else
            qCritical() << "Failed to parse reply: Content-Type = " << contentType
                        << ", Size = " << reply->size()
                        << ", URI  = " << reply->request().url();
    }
    else
       qWarning() << "Request failed: " << reply->errorString();

    requests.removeAll(reply);
}

/********* Requests **********/

void Camera::takeImage()                { get("exec_takemotion"); }
void Camera::powerOff()                 { get("exec_pwoff"); }

void Camera::requestCamInfo()           { get("get_caminfo"); }
void Camera::requestCapacity()          { get("get_unusedcapacity"); }
void Camera::requestConnectMode()       { get("get_connectmode"); }
void Camera::requestImageList(bool rsv) { get(rsv ? "get_rsvimglist" : "get_imglist"); }


void Camera::requestImage(QString name, QSize resolution)
{

}

void Camera::switchCamMode(CamMode mode)
{
    QPair<QString, QString> params;

    switch (mode) {
        case PLAY:
            params["mode"] = "play";
            break;
        case RECORD:
            params["mode"] = "rec";
            params["lvqty"] = liveViewQuality;
            break;
        case SHUTTER:
            params["mode"] = "shutter";
            break;
    }

    get("switch_cammode", params);
}

/*********** Reply parsers ************/

void Camera::parseXml(QString cgi, QDomDocument *body)
{
    if      (cgi == "get_unusedcapacity") parseCapacity(body);
    else if (cgi == "get_commandlist")    parseCommandList(body);
    else if (cgi == "get_camprop")        parseProperties(body);
    else if (cgi == "set_camprop")        parseProperties(body);
    else if (cgi == "get_connectmode")    parseConnectMode(body);
    else if (cgi == "exec_takemotion")    parseTakeMotion(body);
    else if (cgi == "exec_takemisc")      parseTakeMisc(body);
}

void Camera::parseList(QByteArray *body)
{
    QList<QString> &list;
    if      (cgi == "get_imglist")        list = imageList;
    else if (cgi == "get_rsvimglist")     list = reservedImageList;


}

void Camera::parseImage(QByteArray *body)
{

}

void Camera::parseCamInfo(QDomDocument *body)
{
    QDomElement camInfo = body->firstChildElement("caminfo");
    if (!camInfo.isNull()) {
        QDomElement model = camInfo.firstChildElement("model");

        camModel = model.text();
    }
}

void Camera::parseCapacity(QDomDocument *body)
{
    QDomElement camInfo = body->firstChildElement("caminfo");
    if (!camInfo.isNull()) {
        QDomElement model = camInfo.firstChildElement("model");

        camModel = model.text();
    }
}

void Camera::parseCommandList(QDomDocument *body)
{
    commandList = *body;
}

void Camera::parseConnectMode(QDomDocument *body)
{

}
