#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <QByteArray>
#include <QDateTime>

class OiCamera;

class OiImage : public QImage
{
    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(QDateTime date READ dateTime)
    Q_PROPERTY(int attributes READ attributes)
    Q_PROPERTY(int size READ size)
    Q_PROPERTY(bool marked READ marked WRITE setMarked)

    public:
        OiImage(QByteArray line, bool marked = false, OiCamera *cam = NULL);

        ~OiImage();

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

        bool operator ==(const OiImage &);

    protected:
        void decodeDateTime(int dat, int tim);

        OiCamera *cam;

        QImage data;
        QImage thumbnail;

        QString path;
        QDateTime date;

        int attributes;
        int size;
        bool marked;
};

#endif // IMAGE_H
