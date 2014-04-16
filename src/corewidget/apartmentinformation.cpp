#include "apartmentinformation.h"
#include "ui_apartmentinformation.h"

#include <QLabel>
#include <QStringList>
#include <QCompleter>
#include <QDate>

#include "globalsbase.h"


BrainComboBox::BrainComboBox(int aId, QWidget *parent):
    QComboBox(parent)
  ,mId(aId)
  , mIdValue(-1)
  ,mIdObject(-1)
  ,mMultiSelect(false)
{
    ResponseType vValues = execQuery(QString("SELECT id, name FROM dictionaries WHERE parent = %1").arg(mId));
    QStringList vHint;
    addItem(TRANSLATE("-----"), -1);

    if (aId == 209)
    {
        int vCurrentYear = QDate::currentDate().year();
        int vMaxYear = vCurrentYear + 5;
        int vMinYear = 1870;

        for (int i = vMaxYear; i >= vMinYear; --i)
        {
            if (i > vCurrentYear)
            {
                vHint << QString(TRANSLATE("сдача %1")).arg(i);
                addItem(vHint.last(), i);
            }
            if (i < vCurrentYear)
            {
                vHint << QString("%1").arg(i);
                addItem(vHint.last(), i);
            }
            if (i == vCurrentYear)
            {
                vHint << QString(TRANSLATE("сдача %1")).arg(i);
                addItem(vHint.last(), i+1000);
                vHint << QString("%1").arg(i);
                addItem(vHint.last(), i);
            }

        }
    }
    else
    {
        for (int k = 0; k < vValues.count(); ++k)
        {
            vHint << vValues[k]["name"].toString();
            addItem(vHint.last(), vValues[k]["id"].toInt());
        }
    }

    setCompleter(new QCompleter(vHint));
    setConnect();
}

BrainComboBox::~BrainComboBox()
{

}

void BrainComboBox::load(int aIdObject)
{
    setDisconnect();
    mIdValue = -1;
    mIdObject = aIdObject;

    ResponseType vQuery;
    if (mId == 209)
    {
        vQuery = execQuery(QString("SELECT optionvalue.id as id, value_fk FROM optionvalue WHERE object_fk = %1 AND value_fk > 1800")
                                    .arg(aIdObject)
                                    );
    }
    else
    {
        vQuery = execQuery(QString("SELECT optionvalue.id as id, name, value_fk FROM dictionaries INNER JOIN optionvalue ON optionvalue.value_fk = dictionaries.id WHERE object_fk = %1 AND value_fk IN (SELECT id FROM dictionaries WHERE parent = %2)")
                                    .arg(aIdObject)
                                    .arg(mId));
    }
    if (vQuery.count() > 0)
    {

        mIdValue = vQuery[0]["id"].toInt();
        int vValue = vQuery[0]["value_fk"].toInt();
        if (mId == 209)
        {
            int vCurrentYear = QDate::currentDate().year();
            if (vValue > 2100 && (vValue - 1000 < vCurrentYear))
            {
                vValue -= 1000;
            }

        }
        setCurrentIndex(findData(vValue));
    }
    else
    {
        setCurrentIndex(0);
    }
    setConnect();
}

void BrainComboBox::save()
{
    setDisconnect();
    int vNewId = itemData(currentIndex()).toInt();
    if (vNewId < 0)
    {
        execQuery(QString("DELETE FROM optionvalue WHERE id = %1")
                  .arg(mIdValue));
        mIdValue = -1;
    }
    else
    {
        if (mIdValue < 0)
        {
            execQuery(QString("INSERT INTO optionvalue (value_fk, object_fk) values (%1, %2)")
                      .arg(vNewId)
                      .arg(mIdObject));
            mIdValue = execQuery(QString("SELECT id FROM optionvalue WHERE value_fk = %1 AND object_fk = %2")
                                 .arg(vNewId)
                                 .arg(mIdObject))[0]["id"].toInt();
        }
        else
        {
            execQuery(QString("UPDATE optionvalue SET value_fk = %1 WHERE id = %2")
                          .arg(vNewId)
                          .arg(mIdValue));
        }
    }
    setConnect();
}

void BrainComboBox::setMultiSelect(bool aFlag)
{
    mMultiSelect = aFlag;
    setConnect();
}

bool BrainComboBox::isMultiSelect()
{
    return mMultiSelect;
}

QList<int> BrainComboBox::values()
{
    QList<int> vResult;
    for (int i = 1; i < count(); ++i)
    {
        if (!itemIcon(i).isNull())
        {
            if (mId == 209)
            {
                int vCurrentYear = QDate::currentDate().year();
                int vValue = itemData(i).toInt();
                if (vValue < vCurrentYear)
                {
                    vResult.append(vValue);
                    vResult.append(vValue+1000);
                }
                else
                {
                    vResult.append(vValue);
                }
            }
            else
            {
                vResult.append(itemData(i).toInt());
            }
        }
    }
    return vResult;
}

void BrainComboBox::setItem(int aIndex)
{
    if (count() > aIndex && aIndex > 0)
    {
        if (itemIcon(aIndex).isNull())
        {
            setItemIcon(aIndex, QIcon(":/select/select-ok.png"));
        }
        else
        {
            setItemIcon(aIndex, QIcon());
        }
    }
}

void BrainComboBox::setConnect()
{
    //connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
    if (mMultiSelect)
    {
        connect(this, SIGNAL(activated(int)), this, SLOT(setItem(int)));
    }
}

void BrainComboBox::setDisconnect()
{
    //disconnect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(save()));
    if (mMultiSelect)
    {
        disconnect(this, SIGNAL(activated(int)), this, SLOT(setItem(int)));
    }
}

int BrainComboBox::value()
{
    return itemData(currentIndex()).toInt();
}




ApartmentInformation::ApartmentInformation(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::ApartmentInformation)
  ,mIdObject(-1)
{
    ui->setupUi(this);
}

ApartmentInformation::~ApartmentInformation()
{
    delete ui;
}

void ApartmentInformation::setNameDictionary(QString aName, int aCountColumn, bool aMultiSelect)
{
    ResponseType vQuery = execQuery(QString("SELECT d.id as \"id\", d.name as \"name\", sort FROM dictionaries as d INNER JOIN modules_dictionaries as md ON md.fk_dictionaries = d.id INNER JOIN modules as m ON md.fk_modules = m.id WHERE m.name = '%1' ORDER BY sort").arg(aName));

    int vCountRow = vQuery.count() / aCountColumn;
    if (vQuery.count() % aCountColumn > 0) vCountRow+=1;

    for (int i = 0; i < aCountColumn; ++i)
    {
        QHBoxLayout* vLayout = new QHBoxLayout();
        vLayout->setObjectName(QString("Layout%1").arg(i));
        ui->centralLayout->addLayout(vLayout);

        QVBoxLayout* vSubLayout1 = new QVBoxLayout();
        vSubLayout1->setObjectName(QString("Layout%1 1").arg(i));
        vLayout->addLayout(vSubLayout1);

        QVBoxLayout* vSubLayout2 = new QVBoxLayout();
        vSubLayout2->setObjectName(QString("Layout%1 2").arg(i));
        vLayout->addLayout(vSubLayout2);
        QSpacerItem *horizontalSpacer= new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        vLayout->addSpacerItem(horizontalSpacer);
        for (int j = 0; j < vCountRow; ++j)
        {

            if (vQuery.count() <= i*vCountRow + j) break;
            int vId = vQuery[i*vCountRow + j]["id"].toInt();
            QString vName = vQuery[i*vCountRow + j]["name"].toString();

            QLabel* vpLabel = new QLabel(vName);
            BrainComboBox* vpBox = new BrainComboBox(vId, this);
            mBoxes.append(vpBox);
            vpBox->setObjectName(QString("%1").arg(vId));
            vpBox->setMultiSelect(aMultiSelect);

            vSubLayout1->addWidget(vpLabel);
            vSubLayout2->addWidget(vpBox);
        }


    }
}

void ApartmentInformation::load(int aIdObject)
{
    mIdObject = aIdObject;
    foreach (BrainComboBox* vpBox, mBoxes)
    {
        vpBox->load(aIdObject);
    }
}

void ApartmentInformation::save()
{
    foreach (BrainComboBox* vpBox, mBoxes)
    {
        vpBox->save();
    }
}

bool ApartmentInformation::isSave()
{
    return execQuery(QString("SELECT * FROM optionvalue WHERE object_fk = %1").arg(mIdObject)).count();
}

void ApartmentInformation::setEnable(bool aEnable)
{
    foreach (BrainComboBox* vpBox, mBoxes)
    {
        vpBox->setEnabled(aEnable);
    }
}

QString ApartmentInformation::sqlWhere()
{
    QStringList vWheres;
    int i = 0;
    bool vFlag = false;
    QString vWhereString = QString("SELECT ov0.object_fk FROM ");
    QString vWhereString2;
    foreach (BrainComboBox* vpBox, mBoxes)
    {
        QList<int> vIds = vpBox->values();
        if (vIds.count())
        {
            vFlag = true;
            QStringList vSubWhere;
            foreach(int vId, vIds)
            {
                vSubWhere << QString("ov%1.value_fk = %2").arg(i).arg(vId);
            }
            if (i == 0)
            {
                vWhereString += "optionvalue as ov0 ";
                vWhereString2 = QString("WHERE %1").arg(vSubWhere.join(" OR "));
            }
            else
            {

                vWhereString += QString("INNER JOIN optionvalue as ov%1 ON ov0.object_fk = ov%1.object_fk AND (%2)").arg(i).arg(vSubWhere.join(" OR "));
            }
            i+= 1;
            //vWheres << "(" + vSubWhere.join(" OR ") + ")";
        }
    }
    if (vFlag) return QString("objects.id in (%1 %2)").arg(vWhereString).arg(vWhereString2);
    else return "";
    return vWheres.join(" AND ");
}

QString ApartmentInformation::joinWhere()
{
    return QString("INNER JOIN optionvalue ON objects.id = optionvalue.object_fk");
}
