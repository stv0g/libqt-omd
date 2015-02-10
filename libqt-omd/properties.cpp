/** Oi Property Class
 *
 * @copyright	2015 Steffen Vogel
 * @license     http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author      Steffen Vogel <post@steffenvogel.de>
 * @link        https://dev.0l.de/wiki/projects/omd/start
 * @package     libqt-omd
 */
/*
 * This file is part of libqt-omd
 *
 * libqt-omd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libqt-omd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libqt-omd. If not, see <http://www.gnu.org/licenses/>.
 */

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
