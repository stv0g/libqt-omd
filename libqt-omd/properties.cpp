#include <QtDebug>

#include "properties.h"

OiProperties::OiProperties(QDomNode desclist)
{
    QDomNode desc = desclist.firstChild();
    while (!desc.isNull()) {
        OiProperty prop(desc);

        if (prop.isValid())
            properties[prop.key] = prop;

        desc = desc.nextSibling();
    }
}

OiProperty::OiProperty(QDomNode desc)
{
    key   = desc.firstChildElement("propname").text();
    value = desc.firstChildElement("value").text();
    valid = desc.firstChildElement("enum").text().split(' ');

    QString attr = desc.firstChildElement("attribute").text();
    if (attr.contains("set"))
        flags |= PROP_SETABLE;
    if (attr.contains("get"))
        flags |= PROP_GETABLE;
    if (valid.contains(value))
        flags |= PROP_VALID;
}

OiProperty & OiProperty::operator =(const QString &newValue)
{
    if (valid.contains(newValue)) {
        value = newValue;
        flags |= PROP_CHANGED;
    }

    return *this;
}

bool OiProperty::isValid(QString check)
{
   return valid.contains(check.isEmpty() ? value : check);
}
