#ifndef CAMERAPROPERTIES_H
#define CAMERAPROPERTIES_H

#include <QString>
#include <QStringList>

#include "camera.h"

class CameraProperty;

class CameraProperties
{
public:
    CameraProperties(QDomNode desclist);

    CameraProperty& operator[](const QString &key);

protected:

    Camera *cam;

    QMap<QString, CameraProperty> properties;

};

class CameraProperty
{
    friend class CameraProperties;

    enum {
        PROP_VALID      = 1,
        PROP_CHANGED    = 2,
        PROP_SETABLE    = 4,
        PROP_GETABLE    = 8,
    };

public:

    CameraProperty& operator =(const QString &value);
    operator QString();

    bool isValid(QString value = QString);

signals:
    void propertyChanged(QString key, QString value);


protected:
    CameraProperty(QDomNode desc);

    QStringList valid;

    int flags;

    QString key;
    QString value;
};

#endif // CAMERAPROPERTIES_H
