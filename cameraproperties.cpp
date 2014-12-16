#include <QtDebug>

#include "cameraproperties.h"

CameraProperties::CameraProperties(QDomNode desclist)
{
    QDomNode desc = desclist.firstChild();
    while (!desc.isNull()) {
        CameraProperty prop(desc);

        if (prop.isValid()) {
            properties[prop.key] = prop;
            qDebug() <<
        }

        desc = desc.nextSibling();
    }
}


CameraProperty::CameraProperty(QDomNode desc) :
{
    key   = desc.firstChildElement("propname").text();
    value = desc.firstChildElement("value").text();
    valid = desc.firstChildElement("enum").text().split(' ');

    QString attr = desc.firstChildElement("attribute").text();
    if (attr.contains("set"))
        flags |= PROP_SET;
    if (attr.contains("get"))
        flags |= PROP_GET;
    if (valid.contains(value))
        flags |= PROP_VALID;
}

CameraProperty& CameraProperty::operator =(const QString &newValue)
{
    if (valid.contains(newValue)) {
        value = newValue;
        flags |= PROP_CHANGED;

    }

    emit propertyChanged(key, value);
}

bool CameraProperty::isValid(QString check)
{
   return valid.contains(check.isEmpty() ? value : check);
}
