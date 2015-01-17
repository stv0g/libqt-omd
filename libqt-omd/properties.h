#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QString>
#include <QStringList>

#include "camera.h"

class Property;

class Properties
{
public:
    Properties(QDomNode desclist);

    Property& operator[](const QString &key);

protected:
    Camera *cam;

    QMap<QString, Property> properties;
};

class Property
{
    friend class Properties;

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
