#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QString>
#include <QStringList>

#include "camera.h"

class OiProperty;

class OiProperties
{
public:
    OiProperties(QDomNode desclist);

    OiProperty& operator[](const QString &key);

protected:
    OiCamera *cam;

    QMap<QString, OiProperty> properties;
};

class OiProperty
{
    friend class OiProperties;

    enum {
        PROP_VALID      = 1,
        PROP_CHANGED    = 2,
        PROP_SETABLE    = 4,
        PROP_GETABLE    = 8,
    };

public:
    OiProperty() { }
    OiProperty(QDomElement desc);

    OiProperty& operator =(const QString &value);
    operator QString();

    bool isValid(QString value = QString());

protected:
    OiProperty(QDomNode desc);

    QStringList valid;

    int flags;

    QString key;
    QString value;
};

#endif // PROPERTIES_H
