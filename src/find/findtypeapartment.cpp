#include "findtypeapartment.h"
#include "ui_findtypeapartment.h"

#include "globalsbase.h"

FindTypeApartment::FindTypeApartment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindTypeApartment)
{
    ui->setupUi(this);

    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->mpMinRoom->setVisible(false);
    ui->mpMaxRoom->setVisible(false);

    QStringList vDicts;
    vDicts << "apartment" << "material" << "fund";

    mBoxes << ui->mpApartment << ui->mpMaterial << ui->mpFund;
    for (int i =0; i < vDicts.count(); ++i)
    {
         int vDictId = execQuery(
                    QString("SELECT id FROM static_dictionaries WHERE parent is NULL AND \"name\" = '%1'")
                     .arg(vDicts.at(i)))[0]["id"].toInt();
        ResponseType vQuery = execQuery(
                    QString("SELECT id, \"name\" FROM static_dictionaries WHERE parent = %1")
                    .arg(vDictId));
        ResponseRecordType vRecord;
        mBoxes[i]->addItem(TRANSLATE("не выбрано"), -1);
        foreach (vRecord, vQuery)
        {
            mBoxes[i]->addItem(vRecord["name"].toString(), vRecord["id"].toInt());
        }
    }
    connect(&mMapper, SIGNAL(mapped(int)), this, SLOT(selectComboBox(int)));
    for (int i = 0; i < mBoxes.count(); ++i)
    {
        connect(mBoxes[i], SIGNAL(activated(int)), &mMapper, SLOT(map()));
        mMapper.setMapping(mBoxes[i], i);
    }
}

FindTypeApartment::~FindTypeApartment()
{
    delete ui;
}

/*int FindTypeApartment::minRooms()
{
    return ui->mpMinRoom->text().toInt();
}

int FindTypeApartment::maxRooms()
{
    return ui->mpMaxRoom->text().toInt();
}*/

int FindTypeApartment::minLevel()
{
    return ui->mpMinLevel->text().toInt();
}

int FindTypeApartment::maxLevel()
{
    return ui->mpMaxLevel->text().toInt();
}

int FindTypeApartment::minLevels()
{
    return ui->mpMinLevels->text().toInt();
}

int FindTypeApartment::maxLevels()
{
    return ui->mpMaxLevels->text().toInt();
}

QList<int> FindTypeApartment::material()
{
    return idsComboBox(ui->mpMaterial);
}

QList<int> FindTypeApartment::apartment()
{
    return idsComboBox(ui->mpApartment);
}

QList<int> FindTypeApartment::fund()
{
    return idsComboBox(ui->mpFund);
}

QString FindTypeApartment::sqlWhere()
{

    QStringList vMaterial;
    QStringList vApartment;
    QStringList vFund;

    foreach (int vId, material()) {
        vMaterial << QString("typeapartment.material_fk = %1").arg(vId);
    }
    foreach (int vId, apartment()) {
        vApartment << QString("typeapartment.apartment_fk = %1").arg(vId);
    }
    foreach (int vId, fund()) {
        vFund << QString("typeapartment.fund_fk = %1").arg(vId);
    }

    QStringList vWheres;
    vWheres
            //<< (vMaterial >= 0 ? QString("typeapartment.material_fk = %1").arg(vMaterial) : QString(""))
            //<< (vApartment >= 0 ? QString("typeapartment.apartment_fk = %1").arg(vApartment) : QString(""))
            //<< (vFund >= 0 ? QString("typeapartment.fund_fk = %1").arg(vFund) : QString(""))
            << "(" + vMaterial.join(" OR ") + ")"
            << "(" + vApartment.join(" OR ") + ")"
            << "(" + vFund.join(" OR ") + ")"
            //<< (minRooms() > 0 ? QString("typeapartment.rooms >= %1").arg(minRooms()) : QString(""))
            //<< (maxRooms() > 0 ? QString("typeapartment.rooms <= %1").arg(maxRooms()) : QString(""))
            << (minLevel() > 0 ? QString("typeapartment.level >= %1").arg(minLevel()) : QString(""))
            << (maxLevel() > 0 ? QString("typeapartment.level <= %1").arg(maxLevel()) : QString(""))
            << (minLevels() > 0 ? QString("typeapartment.maxlevel >= %1").arg(minLevels()) : QString(""))
            << (maxLevels() > 0 ? QString("typeapartment.maxlevel <= %1").arg(maxLevels()) : QString(""));

    vWheres.removeAll(QString(""));
    vWheres.removeAll(QString("()"));


    return QString("(%1)").arg(vWheres.join(QString(" AND ")));
}

QString FindTypeApartment::joinWhere()
{
    return QString("INNER JOIN typeapartment ON objects.id = typeapartment.object_fk");
}

QList<int> FindTypeApartment::idsComboBox(QComboBox* apBox)
{
    QList<int> vResult;
    for (int i = 0; i < apBox->count(); ++i)
    {
        if (!apBox->itemIcon(i).isNull())
        {
            vResult.append(apBox->itemData(i).toInt());
        }
    }
    return vResult;
}

void FindTypeApartment::clickItemComboBox(QComboBox *apBox, int aIndex)
{
    if (apBox->count() > aIndex && aIndex >= 0)
    {
        if (apBox->itemIcon(aIndex).isNull())
        {
            apBox->setItemIcon(aIndex, QIcon(":/select/select-ok.png"));
        }
        else
        {
            apBox->setItemIcon(aIndex, QIcon());
        }
    }
}

 void FindTypeApartment::selectComboBox(int aIndex)
 {
     clickItemComboBox(mBoxes[aIndex], mBoxes[aIndex]->currentIndex());
 }
