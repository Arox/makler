#include "typeapartment.h"
#include "ui_typeapartment.h"

#include <QList>
#include <QMap>
#include <QVariant>
#include <QStringList>
#include <QTime>

#include "globalsbase.h"

TypeApartment::TypeApartment(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::TypeApartment),
    mId(-1),
    mIdObjects(-1)
{
    ui->setupUi(this);
    ui->mpRooms->setVisible(false);
    ui->label_7->setVisible(false);

    QStringList vDicts;
    vDicts << "apartment" << "material" << "fund";

    QList<QComboBox*> vBoxes;
    vBoxes << ui->mpApartment << ui->mpMaterial << ui->mpFond;
    for (int i =0; i < vDicts.count(); ++i)
    {
        ResponseType vQuery = execQuery(
                    QString("SELECT id, \"name\" FROM static_dictionaries WHERE parent IN (SELECT id FROM static_dictionaries WHERE parent is NULL AND \"name\" = '%1')")
                     .arg(vDicts.at(i)));
        ResponseRecordType vRecord;
        vBoxes[i]->addItem(TRANSLATE("не выбрано"), -1);
        foreach (vRecord, vQuery)
        {
            vBoxes[i]->addItem(vRecord["name"].toString(), vRecord["id"].toInt());
        }
    }
}

TypeApartment::~TypeApartment()
{
    delete ui;
}

void TypeApartment::load(int aIdObjects)
{
    mIdObjects = aIdObjects;
    MainWidget::load();
    ResponseType vResponse = execQuery(
                QString("SELECT id, material_fk, apartment_fk, fund_fk, level, maxlevel, rooms FROM typeapartment WHERE object_fk = %1")
                .arg(mIdObjects));

    if (vResponse.count())
    {
        ResponseRecordType vQuery = vResponse[0];
        mId = vQuery["id"].toInt();
        QStringList vFields;
        vFields << "apartment_fk" << "material_fk" << "fund_fk";
        QList<QComboBox*> vBoxes;
        vBoxes << ui->mpApartment << ui->mpMaterial << ui->mpFond;
        for (int i = 0; i < vFields.count(); ++i)
        {
            if (!vQuery[vFields[i]].isNull())
            {
                for (int j = 0; i < vBoxes[i]->count(); ++j)
                {
                    if (vBoxes[i]->itemData(j) == vQuery[vFields[i]])
                    {
                        vBoxes[i]->setCurrentIndex(j);
                        break;
                    }
                }
            }
        }
        ui->mpLevel->setText(vQuery["level"].toString());
        ui->mpLevels->setText(vQuery["maxlevel"].toString());
        ui->mpRooms->setText(vQuery["rooms"].toString());
        MainWidget::save();
    }
    else
    {
        ui->mpLevel->setText("");
        ui->mpLevels->setText("");
        ui->mpRooms->setText("");
        ui->mpApartment->setCurrentIndex(0);
        ui->mpFond->setCurrentIndex(0);
        ui->mpMaterial->setCurrentIndex(0);
    }
}

void TypeApartment::save()
{
    if (mId < 0)
    {
        execQuery(QString("INSERT INTO typeapartment (object_fk) VALUES (%1)")
                  .arg(mIdObjects));
        mId = execQuery(
                    QString("SELECT id FROM typeapartment WHERE object_fk = %1")
                    .arg(mIdObjects))[0]["id"].toInt();
    }
    execQuery(QString("UPDATE typeapartment SET apartment_fk = %1, material_fk = %2, fund_fk = %3, level = %4, maxlevel = %5, rooms = %6 WHERE id = %7")
              .arg(ui->mpApartment->currentIndex() > 0 ? ui->mpApartment->itemData(ui->mpApartment->currentIndex()).toString() : "NULL")
              .arg(ui->mpMaterial->currentIndex() > 0 ? ui->mpMaterial->itemData(ui->mpMaterial->currentIndex()).toString() : "NULL")
              .arg(ui->mpFond->currentIndex() > 0 ? ui->mpFond->itemData(ui->mpFond->currentIndex()).toString() : "NULL")
              .arg(ui->mpLevel->text().isEmpty() ? "0" : ui->mpLevel->text())
              .arg(ui->mpLevels->text().isEmpty() ? "0" : ui->mpLevels->text())
              .arg(ui->mpRooms->text().isEmpty() ? "0" : ui->mpRooms->text())
              .arg(mId));
    MainWidget::save();
}

void TypeApartment::setEnable(bool aEnable)
{
    ui->mpApartment->setEnabled(aEnable);
    ui->mpFond->setEnabled(aEnable);
    ui->mpLevel->setEnabled(aEnable);
    ui->mpLevels->setEnabled(aEnable);
    ui->mpMaterial->setEnabled(aEnable);
    ui->mpRooms->setEnabled(aEnable);
}
