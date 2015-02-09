#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QString>
#include <QStringList>

#include "camera.h"
#include "image.h"

namespace Oi {
    class Property;
    class Properties;
}

class Oi::Properties
{
public:
    Properties(QDomNode desclist);

    Oi::Property& operator[](const QString &key);

protected:
    Oi::Camera *cam;

    QMap<QString, Oi::Property> properties;
};

class Oi::Property
{
    friend class Oi::Properties;

    enum {
        PROP_VALID      = 1,
        PROP_CHANGED    = 2,
        PROP_SETABLE    = 4,
        PROP_GETABLE    = 8,
    };

public:
    Property() { }
    Property(QDomElement desc);

    Property& operator =(const QString &value);
    operator QString();

    bool isValid(QString value = QString());

protected:
    Property(QDomNode desc);

    QStringList valid;

    int flags;

    QString key;
    QString value;
};

#endif // PROPERTIES_H
