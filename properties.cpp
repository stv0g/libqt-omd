#include <QtDebug>

#include "properties.h"

Properties::Properties(QDomNode desclist)
{
    QDomNode desc = desclist.firstChild();
    while (!desc.isNull()) {
        Property prop(desc);

        if (prop.isValid()) {
            properties[prop.key] = prop;
            qDebug() <<
        }

        desc = desc.nextSibling();
    }
}


Property::Property(QDomNode desc) :
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

Property& Property::operator =(const QString &newValue)
{
    if (valid.contains(newValue)) {
        value = newValue;
        flags |= PROP_CHANGED;

    }

    emit propertyChanged(key, value);
}

bool Property::isValid(QString check)
{
   return valid.contains(check.isEmpty() ? value : check);
}
