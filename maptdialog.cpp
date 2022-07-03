#include "maptdialog.h"
#include "server/component.h"
#include "stage1/base.h"
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QRect>
maptDialog::maptDialog(QWidget *parent)
    : QDialog(parent)
{

    setWindowTitle(tr("MAPTranslator"));
//    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    QPushButton *close = new QPushButton(tr("Close"));
    connect(close, &QPushButton::clicked, this, &maptDialog::done);
    QPushButton *dones = new QPushButton(tr("Transvert"));
    connect(dones, &QPushButton::clicked, this, &maptDialog::transMap);
    QPushButton *seek1 = new QPushButton(tr("Find.."));
    connect(seek1, &QPushButton::clicked, this, &maptDialog::find1);
    QPushButton *seek2 = new QPushButton(tr("Find.."));
    connect(seek2, &QPushButton::clicked, this, &maptDialog::find2);

    QLabel *capline1 = new QLabel(tr("Old map:"));
    capline1->setMinimumSize(70, 30);
    QLabel *capline2 = new QLabel(tr("New map:"));
    capline2->setMinimumSize(70, 30);

    line1 = new QLineEdit();
    line2 = new QLineEdit();

    QVBoxLayout *clay = new QVBoxLayout(this);

    QHBoxLayout *llay1 = new QHBoxLayout();
    llay1->addWidget(capline1);
    llay1->addWidget(line1, 50);
    llay1->addWidget(seek1);
    llay1->setSpacing(10);

    QHBoxLayout *llay2 = new QHBoxLayout();
    llay2->addWidget(capline2);
    llay2->addWidget(line2, 50);
    llay2->addWidget(seek2);
    llay2->setSpacing(10);

    QHBoxLayout *llay3 = new QHBoxLayout();
    llay3->addStretch(2);
    llay3->addWidget(close);
    llay3->addStretch(4);
    llay3->addWidget(dones);
    llay3->addStretch(2);


    clay->setSpacing(30);
    clay->addLayout(llay1, 5);
    clay->addLayout(llay2, 5);
    clay->addStretch(2);
    clay->addLayout(llay3, 5);

    this->setLayout(clay);
}

void maptDialog::find1()
{
    QString fn;
    fn = QFileDialog::getOpenFileName(this, tr("Seek the old map"), "", tr("Map(Old) Files (*.dat)"));
    if(fn.isNull()) return ;
    line1->setText(fn);
    return ;
}

void maptDialog::find2()
{
    QString fn;
    fn = QFileDialog::getSaveFileName(this, tr("Save new map as"), "", tr("Json Files (*.json)"));
    if(fn.isNull()) return ;
    line2->setText(fn);
    return ;
}

void maptDialog::transMap()
{
    QString from = line1->text();
    QString to = line2->text();
    if(from.isNull()||from.isEmpty()) return ;
    if(to.isNull()||to.isEmpty()) return ;
    QFile fromf(from), tof(to);
    if(!fromf.open(QFile::ReadOnly)) return ;
    if(!tof.open(QFile::NewOnly))
        if(!tof.open(QFile::WriteOnly)) return ;
    QDataStream in(&fromf);
    in.setByteOrder(QDataStream::LittleEndian);
    quint32 node, szN, szM;
    in >> szN >> szM;
    GameMainMap *newv = new GameMainMap(szN, szM);
    newv->map.reserve(szN * szM);
    for (quint32 i = 0; i < szN * szM; i++)
    {
        MapNode tmp;
        in >> node;
        if (node & HARD_WALL) {
            auto tmpV = Entity(loca(newv->geti(i), newv->getj(i)), inf, HARDWALL).toVariant();
            tmp.resideEntities.push_back(*tmpV);
            delete tmpV;
        }
        if (node & SOFT_WALL) {
            auto tmpV = Entity(loca(newv->geti(i), newv->getj(i)), 0, SOFTWALL).toVariant();
            tmp.resideEntities.push_back(*tmpV);
            delete tmpV;
        }
        if (node & SPEED_UP) {
            auto tmpV = ItemEntity(loca(newv->geti(i), newv->getj(i)), (node >> 16), SPEED).toVariant();
            tmp.resideEntities.push_back(*tmpV);
            delete tmpV;
        }
        if (node & LEVEL_UP) {
            auto tmpV = ItemEntity(loca(newv->geti(i), newv->getj(i)), (node >> 16), LEVEL).toVariant();
            tmp.resideEntities.push_back(*tmpV);
            delete tmpV;
        }
        newv->map.push_back(tmp);
    }

    QJsonObject *out = newv->toJSON();
    QByteArray writeStuff = QJsonDocument(*out).toJson();
    tof.write(writeStuff);
    tof.close();
    fromf.close();
    qDebug()<<"transvert done!";
    delete out;
    delete newv;
    return ;
}
