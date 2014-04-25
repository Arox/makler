#include "addresswidget.h"
#include "ui_addresswidget.h"

#include <QMessageBox>
#include <QTime>
#include <QCompleter>
#include "globalsbase.h"

AddressWidget::AddressWidget(int aState, QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::AddressWidget),
    mId(-1),
    mEnabled(true),
    mState(NORMAL)
{
    ui->setupUi(this);
    connect(ui->mpCity, SIGNAL(currentIndexChanged(int)), this, SLOT(on_mpCity_currentIndexChanged(int)));
    mBoxes << ui->mpCity << ui->mpLocality << ui->mpStreet;
    setState(aState);
}

AddressWidget::~AddressWidget()
{
    delete ui;
}

void AddressWidget::setEnabledWidgets(bool aEnable)
{
    if (aEnable) aEnable = mEnabled;
    ui->mpLandMark->setEnabled(aEnable);
    ui->mpNumber1->setEnabled(aEnable);
    ui->mpNumber2->setEnabled(aEnable);
    ui->mpRoom->setEnabled(aEnable);
}

void AddressWidget::saveCity()
{
    QString vName= ui->mpCity->currentText();
    if (vName.isEmpty()) return;
    if (execQuery(QString("SELECT * FROM address_city WHERE name = '%1'").arg(vName)).count()) return;

    int vType = execQuery(QString("select id from typecity where name='city'"))[0]["id"].toInt();
    execQuery(QString("INSERT INTO address_city (name, parent, type_fk) VALUES('%1', NULL, %2)")
              .arg(vName)
              .arg(vType));
    ui->mpLocality->setEnabled(mEnabled);
    loadCity();
    ui->mpCity->setCurrentIndex(ui->mpCity->findText(vName));
}

void AddressWidget::saveLocality()
{
    int vCity = ui->mpCity->itemData(ui->mpCity->currentIndex()).toInt();
    QString vName = ui->mpLocality->currentText();

    if (vName.isEmpty()) return;
    if (execQuery(QString("SELECT * FROM address_microdistrict WHERE name = '%1' AND city_fk = %2")
                  .arg(vName).arg(vCity)).count()) return;

    execQuery(QString("INSERT INTO address_microdistrict (name, city_fk) VALUES('%1', %2)")
              .arg(vName)
              .arg(vCity));

    loadLocality();
    ui->mpLocality->setCurrentIndex(ui->mpLocality->findText(vName));
}

void AddressWidget::saveStreet()
{
    int vCity = ui->mpCity->itemData(ui->mpCity->currentIndex()).toInt();
    int vLocality = ui->mpLocality->itemData(ui->mpLocality->currentIndex()).toInt();

    QString vName = ui->mpStreet->currentText();

    if (vName.isEmpty()) return;
    if (execQuery(QString("SELECT * FROM address_street WHERE name = '%1' AND city_fk = %2 AND microdistrict_fk = %3")
                  .arg(vName).arg(vCity).arg(vLocality)).count()) return;


    execQuery(QString("INSERT INTO address_street (name, city_fk, microdistrict_fk) VALUES('%1', %2, %3)")
              .arg(vName)
              .arg(vCity)
              .arg(vLocality));
    loadStreet();
    ui->mpStreet->setCurrentIndex(ui->mpStreet->findText(vName));
}

bool AddressWidget::isState(int aState)
{
    return mState & aState;
}

void AddressWidget::loadCity()
{
    ResponseType vResponse = execQuery(QString("select id, name from address_city where type_fk IN (select id from typecity where name='city')"));
    ui->mpCity->blockSignals(true);
    ui->mpCity->clear();
    QStringList vList;
    for (int i = 0; i < vResponse.count(); ++i)
    {
        ui->mpCity->addItem(vResponse[i]["name"].toString(), vResponse[i]["id"]);
        vList << vResponse[i]["name"].toString();
    }

    if (ui->mpCity->count())
    {
        if (isState(FIND) && !isState(MULTISELECT))
        {
            vList.insert(0, "");
            ui->mpCity->insertItem(0, "", -1);
        }
        ui->mpCity->setCurrentIndex(0);
        on_mpCity_currentIndexChanged(0);
    }
    QCompleter* vpComp = new QCompleter(vList, this);
    vpComp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->mpCity->setCompleter(vpComp);
    ui->mpCity->blockSignals(false);
}

void AddressWidget::loadLocality()
{
    int vCity = ui->mpCity->itemData(ui->mpCity->currentIndex()).toInt();
    ResponseType vResponse = execQuery(QString("select id, name from address_microdistrict where city_fk = %1")
                                       .arg(vCity));
    ui->mpLocality->clear();
    QStringList vList;
    for (int i = 0; i < vResponse.count(); ++i)
    {
        ui->mpLocality->addItem(vResponse[i]["name"].toString(), vResponse[i]["id"]);
        vList << vResponse[i]["name"].toString();
    }


    if (ui->mpCity->count())
    {
        ui->mpLocality->setEnabled(mEnabled);
    }
    else
    {
        ui->mpLocality->setEnabled(false);
    }
    if (ui->mpLocality->count())
    {
        if (isState(FIND) && !isState(MULTISELECT))
        {
            vList.insert(0, "");
            ui->mpLocality->insertItem(0, "", -1);
        }
        ui->mpLocality->setCurrentIndex(0);
    }
    QCompleter* vpComp = new QCompleter(vList, this);
    vpComp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->mpLocality->setCompleter(vpComp);

}

void AddressWidget::loadStreet()
{
    int vCity = ui->mpCity->itemData(ui->mpCity->currentIndex()).toInt();
    ResponseType vResponse = execQuery(QString("select id, name from address_street where city_fk = %1")
                                           .arg(vCity));
    ui->mpStreet->clear();
    QStringList vList;
    QList<int> vData;
    ui->mpStreet->addItems(QStringList::fromVector(QVector<QString>(vResponse.count(), "")));
    for (int i = 0; i < vResponse.count(); ++i)
    {
        QString s1 = vResponse[i]["name"].toString();
        int s2 = vResponse[i]["id"].toInt();
        ui->mpStreet->setItemText(i, s1);
        ui->mpStreet->setItemData(i, s2);
        vList << s1;
    }


    if (ui->mpCity->count())
    {
        ui->mpStreet->setEnabled(mEnabled);
    }
    else
    {
        ui->mpStreet->setEnabled(false);
    }

    if (ui->mpStreet->count())
    {
        if (isState(FIND) && !isState(MULTISELECT))
        {
            vList.insert(0, "");
            ui->mpStreet->insertItem(0, "", -1);
        }
        ui->mpStreet->setCurrentIndex(0);
    }
    QCompleter* vpComp = new QCompleter(vList, this);
    vpComp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->mpStreet->setCompleter(vpComp);

}

void AddressWidget::load(int aIdObjects, int aNumber)
{
    MainWidget::load();

    ui->mpLandMark->blockSignals(true);
    ui->mpNumber1->blockSignals(true);
    ui->mpNumber2->blockSignals(true);
    ui->mpRoom->blockSignals(true);
    ui->mpStreet->blockSignals(true);
    ui->mpLocality->blockSignals(true);

    mIdObjects = aIdObjects;


    ResponseType vResponseAddress = execQuery(
                QString("SELECT address.id as id, street_fk, address.microdistrict_fk as microdistrict_fk, number1, number2, room, landmark, address_city.id as city_fk FROM address LEFT JOIN address_street ON address.street_fk = address_street.id LEFT JOIN address_microdistrict ON address_microdistrict.id = address.microdistrict_fk LEFT JOIN address_city ON address_city.id = address_street.city_fk OR address_city.id = address_microdistrict.city_fk LEFT JOIN typecity ON typecity.id = address_city.type_fk AND  typecity.name = 'city' WHERE  object_fk = %1")
                .arg(mIdObjects));
    if (vResponseAddress.count() >= aNumber)
    {
        ResponseRecordType vQuery = vResponseAddress[aNumber - 1];

        ui->mpLandMark->setText(vQuery["landmark"].toString());
        ui->mpNumber1->setText(vQuery["number1"].toString());
        ui->mpNumber2->setText(vQuery["number2"].toString());
        ui->mpRoom->setText(vQuery["room"].toString());
        mId = vQuery["id"].toInt();

        if (!vQuery["city_fk"].isNull()) ui->mpCity->setCurrentIndex(ui->mpCity->findData(vQuery["city_fk"].toInt())); else ui->mpCity->setCurrentIndex(ui->mpCity->findData(-1));
        if (!vQuery["microdistrict_fk"].isNull()) ui->mpLocality->setCurrentIndex(ui->mpLocality->findData(vQuery["microdistrict_fk"].toInt())); else ui->mpLocality->setCurrentIndex(ui->mpLocality->findData(-1));
        if (!vQuery["street_fk"].isNull()) ui->mpStreet->setCurrentIndex(ui->mpStreet->findData(vQuery["street_fk"].toInt())); else ui->mpStreet->setCurrentIndex(ui->mpStreet->findData(-1));

        MainWidget::save();
    }
    else
    {
        ui->mpLandMark->setText("");
        ui->mpNumber1->setText("");
        ui->mpNumber2->setText("");
        ui->mpRoom->setText("");

        ui->mpCity->setCurrentIndex(-1);
        ui->mpLocality->setCurrentIndex(-1);
        ui->mpStreet->setCurrentIndex(-1);
    }

    ui->mpLandMark->blockSignals(false);
    ui->mpNumber1->blockSignals(false);
    ui->mpNumber2->blockSignals(false);
    ui->mpRoom->blockSignals(false);

    ui->mpStreet->blockSignals(false);
    ui->mpLocality->blockSignals(false);
    /*ui->mpCity->blockSignals(false);
    ui->mpLocality->blockSignals(false);
    ui->mpStreet->blockSignals(false);*/
}

void AddressWidget::save()
{
    if (ui->mpStreet->currentIndex() < 0)
    {
        return;
    }

    if (mId < 0)
    {
        qsrand(QTime::currentTime().msec());
        int vRand = qrand() % 1000000;
        execQuery(QString("INSERT INTO address (landmark, object_fk) VALUES ('%1', %2)")
                  .arg(vRand).arg(mIdObjects));
        mId = execQuery(
                    QString("SELECT id FROM address WHERE landmark = '%1' and object_fk = %2")
                    .arg(vRand).arg(mIdObjects))[0]["id"].toInt();
    }

    execQuery(QString("UPDATE address SET street_fk = %1, microdistrict_fk = %2, number1 = '%3', number2 = '%4', room = %5, landmark = '%6' WHERE id = %7")
              .arg(ui->mpStreet->currentIndex() < 0 ? "NULL" : ui->mpStreet->itemData(ui->mpStreet->currentIndex()).toString())
              .arg(ui->mpLocality->currentIndex() < 0 ? "NULL" : ui->mpLocality->itemData(ui->mpLocality->currentIndex()).toString())
              .arg(ui->mpNumber1->text())
              .arg(ui->mpNumber2->text())
              .arg(ui->mpRoom->text().isEmpty() ? "NULL" : ui->mpRoom->text())
              .arg(ui->mpLandMark->text())
              .arg(mId));
    MainWidget::save();
}

void AddressWidget::setEnable(bool aEnable)
{
    ui->mpCity->setEnabled(aEnable);
    mEnabled = aEnable;
    if (!ui->mpCity->count()) aEnable = false;
    ui->mpStreet->setEnabled(aEnable);
    ui->mpLocality->setEnabled(aEnable);
    if (!ui->mpStreet->count()) aEnable = false;
    ui->mpLandMark->setEnabled(aEnable);
    ui->mpNumber1->setEnabled(aEnable);
    ui->mpNumber2->setEnabled(aEnable);
    ui->mpRoom->setEnabled(aEnable);
}


QList<int> AddressWidget::city()
{
    if (isState(MULTISELECT))
    {
        return idsComboBox(ui->mpCity);
    }
    return  idComboBox(ui->mpCity);
}

QList<int> AddressWidget::locality()
{
    if (isState(MULTISELECT))
    {
        return idsComboBox(ui->mpLocality);
    }
    return  idComboBox(ui->mpLocality);
}

QList<int> AddressWidget::street()
{
    if (isState(MULTISELECT))
    {
        return idsComboBox(ui->mpStreet);
    }
    return  idComboBox(ui->mpStreet);
}

int AddressWidget::number1()
{
    return ui->mpNumber1->text().toInt();
}

int AddressWidget::number2()
{
    return ui->mpNumber2->text().toInt();
}

int AddressWidget::room()
{
    return ui->mpRoom->text().toInt();
}

QString AddressWidget::landmark()
{
    return ui->mpLandMark->text();
}

bool AddressWidget::isCity()
{
    return city().count() > 0;
}

bool AddressWidget::isLocality()
{
    return locality().count() > 0;
}

bool AddressWidget::isStreet()
{

    return street().count() > 0;
}

bool AddressWidget::isNumber1()
{
    return number1() > 0;
}

bool AddressWidget::isNumber2()
{
    return number2() > 0;
}

bool AddressWidget::isRoom()
{
    return room() > 0;
}

bool AddressWidget::isLandmark()
{
    return !landmark().isEmpty();
}

void AddressWidget::setState(int aState)
{
    mState = aState;

    if (isState(NORMAL))
    {
        ui->mpCity->setVisible(true);
        ui->mpLandMark->setVisible(true);
        ui->mpLocality->setVisible(true);
        ui->mpNumber1->setVisible(true);
        ui->mpNumber2->setVisible(true);
        ui->mpRoom->setVisible(true);
        ui->mpStreet->setVisible(true);
        ui->label->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_6->setVisible(true);
    }
    if (isState(FIND))
    {
        ui->mpCity->setVisible(true);
        ui->mpLandMark->setVisible(false);
        ui->mpLocality->setVisible(true);
        ui->mpStreet->setVisible(true);


        ui->label_2->setVisible(false);
        ui->label_3->setVisible(false);
        ui->label_6->setVisible(false);

        ui->mpCity->setEditable(true);
        ui->mpStreet->setEditable(true);
        ui->mpLocality->setEditable(true);

        if (isState(FINDROOM))
        {
            ui->mpNumber1->setVisible(true);
            ui->mpNumber2->setVisible(true);
            ui->mpRoom->setVisible(true);

            ui->label->setVisible(true);
        }
        else
        {
            ui->mpNumber1->setVisible(false);
            ui->mpNumber2->setVisible(false);
            ui->mpRoom->setVisible(false);
            ui->label->setVisible(false);
        }
    }

    if (isState(MULTISELECT))
    {
        connect(&mMapper, SIGNAL(mapped(int)), this, SLOT(selectComboBox(int)));
        for (int i = 0; i < mBoxes.count(); ++i)
        {
            connect(mBoxes[i], SIGNAL(activated(int)), &mMapper, SLOT(map()));
            connect(mBoxes[i], SIGNAL(currentIndexChanged(int)), &mMapper, SLOT(map()));
            mMapper.setMapping(mBoxes[i], i);
        }
    }

    loadCity();
}

void AddressWidget::on_mpCity_currentIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->mpLocality->setEnabled(mEnabled);
        ui->mpStreet->setEnabled(mEnabled);
        loadStreet();
        loadLocality();
    }
    else
    {
        ui->mpLocality->setEnabled(false);
        ui->mpStreet->setEnabled(false);
    }
}

bool AddressWidget::canSave()
{
    return (ui->mpStreet->currentIndex() >= 0) && (ui->mpNumber1->text().length());
}

QList<int> AddressWidget::idComboBox(QComboBox* apBox)
{
    QList<int> vResult;
    if (apBox->currentIndex() >= 0)
    {
        if (apBox->itemData(apBox->currentIndex()).toInt() >= 0)
        {
            vResult.append(apBox->itemData(apBox->currentIndex()).toInt());
        }
    }
    return vResult;
}

QList<int> AddressWidget::idsComboBox(QComboBox* apBox)
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

void AddressWidget::clickItemComboBox(QComboBox *apBox, int aIndex)
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

 void AddressWidget::selectComboBox(int aIndex)
 {
     clickItemComboBox(mBoxes[aIndex], mBoxes[aIndex]->currentIndex());
 }
