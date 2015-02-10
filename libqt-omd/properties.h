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
    Oi::Camera *mCam;

    QMap<QString, Oi::Property> mProperties;
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

    QStringList mValid;

    int mFlags;

    QString mKey;
    QString mValue;
};

#endif // PROPERTIES_H
