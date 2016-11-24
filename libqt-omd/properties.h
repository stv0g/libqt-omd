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


#include <QStringList>
#include <QDomNode>

#include "image.h"

namespace Oi {
    class Camera;
    class Property;
    class Properties;
}

class Oi::Properties :
    public QMap<QString, Oi::Property>
{
public:
    Properties(Oi::Camera *c);

    void parse(QDomNode desclist);
    Oi::Property& operator[](const QString &key);

protected:
    Oi::Camera *mCam;

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

    Property& operator =(const QString &value);

    bool isValid(QString value = QString());

    /* Getter */
    const QString & key() const { return mKey; }
    const QString & value() const { return mValue; }
    const QStringList & valids() const { return mValid; }

protected:
    Property(Oi::Camera *c, QDomNode desc);

    Oi::Camera *mCam;

    QStringList mValid;

    int mFlags;

    QString mKey;
    QString mValue;
};

#endif // PROPERTIES_H
