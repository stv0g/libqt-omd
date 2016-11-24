#ifndef IMAGETABLEMODEL_H
#define IMAGETABLEMODEL_H

#include <QAbstractTableModel>

namespace Oi {
    class Camera;
    class ImageListModel;
}

class Oi::ImageListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ImageListModel(Camera *cam, QObject *parent = 0);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    Oi::Camera *mCam;

private:
};

#endif // IMAGELISTMODEL_H
