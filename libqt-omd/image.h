/** Oi Image Class
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

#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <QByteArray>
#include <QDateTime>


namespace Oi {
    class Camera;
    class Image;
}

class Oi::Image : public QImage
{
    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(QDateTime date READ dateTime)
    Q_PROPERTY(int attributes READ attributes)
    Q_PROPERTY(int size READ size)
    Q_PROPERTY(bool marked READ marked WRITE setMarked)

    public:
        Image();
        Image(const Image &img);
        Image(QString line, bool marked = false, Oi::Camera *cam = NULL);

        ~Image();

        void load();

        /* Getter */
        QString     path() const;
        QDateTime   dateTime() const;
        int         attributes() const;
        int         size() const;
        bool        marked() const;

        /* Setter */
        void        setMarked(bool mark);

        /* Operators */

        bool operator ==(const Image &);

    protected:
        void decodeDateTime(int dat, int tim);

        Oi::Camera *mCam;

        QString mPath;
        QDateTime mDate;

        int mAttributes;
        int mSize;
        bool mMarked;
};

#endif // IMAGE_H
