#include "camera.h"
#include "image.h"

using namespace Oi;

#define BITS(x, from, to) ((x >> 9) & ((1 << (to - from)) - 1))

Image::Image() :
    mCam(NULL)
{ }
{
    QByteArrayList fields = line.split(',');

    path       = QString(fields[0]).append(fields[1]);
    size       = fields[2].toInt();
    attributes = fields[3].toInt();

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

    date = QDateTime(
        QDate(year, month, day),
        QTime(hour, minute, sec, msec)
    );
}

void Image::load()
{
    cam->requestImage(path);
}

/* Getter */

QString Image::path() const
{
    return path;
}

QDateTime Image::dateTime() const
{
    return date;
}

int Image::attributes() const
{
    return attributes;
}

int  Image::size() const
{
    return size;
}

bool Image::marked() const
{
    return marked;
}

/* Setter */
void Image::setMarked(bool mark)
{
    marked = m;
}

/* Operators */
bool Image::operator ==(const Image &b)
{
    return (
        ( b.path == path ) &&
        ( b.size == size ) &&
        ( b.attributes == attributes) &&
        ( b.date == date )
    );
}

