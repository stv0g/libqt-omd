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

        QMap<QSize, QImage> mPixels;

        QString mPath;
        QDateTime mDate;

        int mAttributes;
        int mSize;
        bool mMarked;
};

#endif // IMAGE_H
