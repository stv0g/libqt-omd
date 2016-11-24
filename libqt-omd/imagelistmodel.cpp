#include "imagelistmodel.h"
#include "image.h"
#include "camera.h"

using namespace Oi;

ImageListModel::ImageListModel(Camera *cam, QObject *parent) :
    QAbstractTableModel(parent),
    mCam(cam)
{
    connect(cam, &Oi::Camera::imagesUpdated, this, [&](QHash<QString, Image>) {
        emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));
    });
}

QVariant ImageListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
        case 0: return QString(tr("Path"));
        case 1: return QString(tr("Size"));
        case 2: return QString(tr("Marked"));
        case 3: return QString(tr("Date"));
        case 4: return QString(tr("Attributes"));
        default: return QVariant();
    }
}

int ImageListModel::rowCount(const QModelIndex &parent) const
{
    return mCam->images().count();
}

int ImageListModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant ImageListModel::data(const QModelIndex &index, int role) const
{
//    if (!index.isValid())
//        return QVariant();

    const Image &img = mCam->images().values().at(index.row());

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (index.column()) {
        case 0: return img.path();
        case 1: return img.size();
        case 2: return img.marked();
        case 3: return img.dateTime().toString();
        case 4: return img.attributes();
        default: return QVariant();
    }
}
