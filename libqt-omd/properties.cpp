#include <QtDebug>

#include "properties.h"

using namespace Oi;

Properties::Properties(QDomNode desclist)
{
    QDomNode desc = desclist.firstChild();
    while (!desc.isNull()) {
        Property prop(desc);

        if (prop.isValid())
            mProperties[prop.mKey] = prop;

        desc = desc.nextSibling();
    }
}

Property::Property(QDomNode desc)
{
    mKey   = desc.firstChildElement("propname").text();
    mValue = desc.firstChildElement("value").text();
    mValid = desc.firstChildElement("enum").text().split(' ');

    QString attr = desc.firstChildElement("attribute").text();
    if (attr.contains("set"))
        mFlags |= PROP_SETABLE;
    if (attr.contains("get"))
        mFlags |= PROP_GETABLE;
    if (mValid.contains(mValue))
        mFlags |= PROP_VALID;
}

Property & Property::operator =(const QString &newValue)
{
    if (mValid.contains(newValue)) {
        mValue = newValue;
        mFlags |= PROP_CHANGED;
    }

    return *this;
}

bool Property::isValid(QString check)
{
   return mValid.contains(check.isEmpty() ? mValue : check);
}
