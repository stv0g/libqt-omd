#include <QtDebug>
#include <QUrl>
#include <QTcpSocket>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "camera.h"

Camera::Camera() :
    address("192.168.0.10")
{ }

bool Camera::isOnline()
{
    QTcpSocket socket;
    socket.connectToHost(address, 80, QIODevice::ReadOnly);
    return socket.waitForConnected(2000);
}

QNetworkRequest Camera::makeRequest(QString cgi, QPair<QString, QString> params)
{
    QString tpl("http://%1/%2.cgi");
    tpl.arg(address.toString()).arg(cgi);

    QUrl url(tpl);
    url.setQueryItems(params);

    return QNetworkRequest(url);
}

QNetworkReply Camera::get(QString cgi, QPair<QString, QString> params)
{
    QNetworkRequest request = makeRequest(cgi, params);

    return networkManager.get(request);
}

QNetworkReply Camera::post(QString cgi, QPair<QString, QString> params, QDomDocument body)
{
    QNetworkRequest request = makeRequest(cgi, params);

    return networkManager.post(request, body.toByteArray());
}

void Camera::requestFinished(QNetworkReply *reply)
{
    QDomDocument xml;

    if (reply->error() == QNetworkReply::NoError) {
        if (reply->size()) {
            if (xml.setContent(reply->readAll())) {

            }
            else
                qCritical() << "Failed to parse reponse";
        }
        else
            qDebug() << "Empty response";
    }
    else
       qWarning() << "Request failed: " << reply->errorString();
}

void Camera::powerOff()
{
    get("exec_pwoff", params);
}

int Camera::getCapacity()
{
    QNetworkReply *reply = get("get_unusedcapacity", params);

    // FIXME
}

void Camera::takeImage()
