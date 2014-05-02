#include "findtypehome.h"
#include "ui_findtypehome.h"

#include <QStringList>
#include  "globalsbase.h"
FindTypeHome::FindTypeHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindTypeHome)
{
    ui->setupUi(this);

    QStringList vDicts;
    vDicts << "material";

    mBoxes << ui->mpMaterial;
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

FindTypeHome::~FindTypeHome()
{
    delete ui;
}

int FindTypeHome::areaSize()
{
    return ui->mpSize->text().toInt();
}

int FindTypeHome::year()
{
    return ui->mpYear->text().toInt();
}

int FindTypeHome::minRoom()
{
    return ui->mpMinRoom->text().toInt();
}

int FindTypeHome::maxRoom()
{
    return ui->mpMaxRoom->text().toInt();
}

int FindTypeHome::minLevel()
{
    return ui->mpMinLevel->text().toInt();
}

int FindTypeHome::maxLevel()
{
    return ui->mpMaxLevel->text().toInt();
}

QList<int> FindTypeHome::material()
{
    return idsComboBox(ui->mpMaterial);
    //return (ui->mpMaterial->currentIndex() > 0 ? ui->mpMaterial->itemData(ui->mpMaterial->currentIndex()).toInt() : -1);
}

bool FindTypeHome::isAreaSize()
{
    return areaSize() > 0;
}

bool FindTypeHome::isYear()
{
    return year() > 0;
}

bool FindTypeHome::isMinRoom()
{
    return minRoom() > 0;
}

bool FindTypeHome::isMaxRoom()
{
    return maxRoom() > 0;
}

bool FindTypeHome::isMinLevel()
{
    return minLevel() > 0;
}

bool FindTypeHome::isMaxLevel()
{
    return maxLevel() > 0;
}

bool FindTypeHome::isMaterial()
{
    return material().count() >= 0;
}

QString FindTypeHome::sqlWhere()
{
    QStringList vMaterial;

    foreach (int vId, material()) {
        vMaterial << QString("typeapartment.material_fk = %1").arg(vId);
    }
    QStringList vWheres;
    vWheres
            << "(" + vMaterial.join(" OR ") + ")"
            << (isMinRoom() ? QString("typehome.rooms >= %1").arg(minRoom()) : QString(""))
            << (isMaxRoom() ? QString("typehome.rooms <= %1").arg(maxRoom()) : QString(""))
            << (isMinLevel() ? QString("typehome.levels >= %1").arg(minLevel()) : QString(""))
            << (isMaxLevel() ? QString("typehome.levels <= %1").arg(maxLevel()) : QString(""))
            << (isAreaSize() ? QString("typehome.area >= %1").arg(areaSize()) : QString(""))
            << (isYear() ? QString("typehome.year >= %1").arg(year()) : QString(""));

    vWheres.removeAll(QString(""));
    vWheres.removeAll(QString("()"));

    return QString("(%1)").arg(vWheres.join(QString(" AND ")));
}

QString FindTypeHome::joinWhere()
{
    return QString("INNER JOIN typehome ON objects.id = typehome.object_fk");
}

QList<int> FindTypeHome::idsComboBox(QComboBox* apBox)
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

void FindTypeHome::clickItemComboBox(QComboBox *apBox, int aIndex)
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

 void FindTypeHome::selectComboBox(int aIndex)
 {
     clickItemComboBox(mBoxes[aIndex], mBoxes[aIndex]->currentIndex());
 }
