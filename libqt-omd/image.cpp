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

#include "camera.h"
#include "image.h"

using namespace Oi;

#define BITS(x, from, to) ((x >> 9) & ((1 << (to - from)) - 1))

Image::Image() :
    mCam(NULL)
{ }

Image::Image(const Image &img) :
    mCam(img.mCam),
    mPixels(img.mPixels),
    mPath(img.mPath),
    mDate(img.mDate),
    mAttributes(img.mAttributes),
    mSize(img.mSize),
    mMarked(img.mMarked)
{ }

Image::Image(QString line, bool marked, Camera *c) :
    mCam(c),
    mMarked(marked)
{
    QStringList fields = line.split(',');

    mPath       = QString("%1/%2").arg(fields[0]).arg(fields[1]);
    mSize       = fields[2].toInt();
    mAttributes = fields[3].toInt();

    decodeDateTime(
        fields[4].toInt(),
        fields[5].toInt()
    );
}

Image::~Image()
{

}

void Image::decodeDateTime(int dat, int tim)
{
    int year   = BITS(dat, 9, 15) + 1980;
    int month  = BITS(dat, 5, 8);
    int day    = BITS(dat, 0, 4);

    int hour   = BITS(tim, 11, 15);
    int minute = BITS(tim, 5, 10);
    int sec    = BITS(tim, 2, 4);
    int msec   = BITS(tim, 0, 1) * 500;

    mDate = QDateTime(
        QDate(year, month, day),
        QTime(hour, minute, sec, msec)
    );
}

void Image::load()
{
    mCam->requestImage(mPath);
}

/* Getter */

QString Image::path() const
{
    return mPath;
}

QDateTime Image::dateTime() const
{
    return mDate;
}

int Image::attributes() const
{
    return mAttributes;
}

int  Image::size() const
{
    return mSize;
}

bool Image::marked() const
{
    return mMarked;
}

/* Setter */
void Image::setMarked(bool mark)
{
    mMarked = mark;
}

/* Operators */
bool Image::operator ==(const Image &b)
{
    return (
        ( b.mPath == mPath ) &&
        ( b.mSize == mSize ) &&
        ( b.mAttributes == mAttributes) &&
        ( b.mDate == mDate )
    );
}

