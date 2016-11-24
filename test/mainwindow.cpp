#include <QComboBox>

#include <utility>
using std::make_pair;

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Oi::Camera *c, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cam(c),
    imageListModel(c, parent)
{
    ui->setupUi(this);

    ui->imgTable->setModel(&imageListModel);

    connect(cam, &Oi::Camera::propertiesUpdated, this, &MainWindow::updateProperties);
    connect(cam, &Oi::Camera::modelUpdated, [&](QString model) {
        ui->lblModel->setText(QString("Olympus OM-D ") + model);
    });
    connect(cam, &Oi::Camera::imagesUpdated, [&](QHash<QString, Oi::Image> imgs) {
        ui->lblImages->setText(QString("%1").arg(imgs.count()));
    });
    connect(cam, &Oi::Camera::capacityUpdated, [&](unsigned long c) {
        ui->lblCapacity->setText(QString("%1 MB").arg(c / (1 << 20)));
    });
    connect(cam, &Oi::Camera::connected, [&](enum Oi::Camera::ConnectMode m) {
        QLabel *l = ui->lblConnectMode;
        switch (m) {
            case Oi::Camera::CONNECT_PRIVATE: l->setText(tr("Private")); break;
            case Oi::Camera::CONNECT_SHARED:  l->setText(tr("Shared"));  break;
            case Oi::Camera::CONNECT_UNKNOWN: l->setText(tr("Unknown")); break;
        }
    });

    propInfo["touchactiveframe"] = qMakePair(tr(""), tr(""));
    propInfo["takemode"]         = qMakePair(tr("Shooting Mode"), tr(""));
    propInfo["noisereduction"]   = qMakePair(tr("Noise Reduction"), tr(""));
    propInfo["lowvibtime"]       = qMakePair(tr(""), tr(""));
    propInfo["bulbtimelimit"]    = qMakePair(tr(""), tr(""));
    propInfo["digitaltelecon"]   = qMakePair(tr("Digital Tele-converter"), tr(""));
    propInfo["drivemode"]        = qMakePair(tr(""), tr(""));
    propInfo["focalvalue"]       = qMakePair(tr(""), tr(""));
    propInfo["expcomp"]          = qMakePair(tr("Exposure Compensation"), tr(""));
    propInfo["shuttspeedvalue"]  = qMakePair(tr("Shutter Speed"), tr(""));
    propInfo["isospeedvalue"]    = qMakePair(tr("ISO Sensivity"), tr(""));
    propInfo["wbvalue"]          = qMakePair(tr("White Balance"), tr(""));
    propInfo["artfilter"]        = qMakePair(tr("Art Filter"), tr(""));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProperties(Oi::Properties *props)
{
    QGroupBox *wdg = ui->grpProperties;

    /* Create layout if non-existant */
    QFormLayout *layout = qobject_cast<QFormLayout*>(wdg->layout());
    if (!layout) {
        layout = new QFormLayout();
        wdg->setLayout(layout);
    }

    /* Remove old properties */
    while (layout->count()) {
        QLayoutItem *item = layout->takeAt(0);

        delete item->widget();
        delete item;
    }

    for (Oi::Property &prop : props->values()) {
        QLabel *lbl = new QLabel(wdg);
        lbl->setText(propInfo.value(prop.key()).first);
        lbl->setToolTip(propInfo.value(prop.key()).second);

        QComboBox *cbox = new QComboBox(wdg);
        cbox->addItems(prop.valids());
        cbox->setCurrentText(prop.value());

        connect(cbox, &QComboBox::currentTextChanged, [&](QString value) {
           prop = value;
        });

        layout->addRow(lbl, cbox);
    }
}
