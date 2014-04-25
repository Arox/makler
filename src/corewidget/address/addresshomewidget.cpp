#include "addresshomewidget.h"
#include "ui_addresshomewidget.h"

#include <QMessageBox>
#include <QTime>
#include <QCompleter>
#include "globalsbase.h"

AddressHomeWidget::AddressHomeWidget(int aState, QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::AddressHomeWidget),
    mEnabled(true),
    mId(-1),
    mState(NORMAL)
{
    ui->setupUi(this);

    ui->mpStreet->setEnabled(false);
    mBoxes << ui->mpGarden << ui->mpStreet;
    setState(aState);

}
AddressHomeWidget::~AddressHomeWidget()
{
    delete ui;
}

void AddressHomeWidget::setEnabledWidgets(bool aEnable)
{
    if (aEnable) aEnable = mEnabled;
    ui->mpNumber1->setEnabled(aEnable);
    ui->mpNumber2->setEnabled(aEnable);
}

void AddressHomeWidget::saveGarden()
{
    QString vName= ui->mpGarden->currentText();
    if (vName.isEmpty()) return;
    if (execQuery(QString("SELECT * FROM address_city WHERE name = '%1'").arg(vName)).count()) return;

    int vType = execQuery(QString("select id from typecity where name='garden'"))[0]["id"].toInt();
    execQuery(QString("INSERT INTO address_city (name, parent, type_fk) VALUES('%1', NULL, %2)")
              .arg(vName)
              .arg(vType));
    ui->mpStreet->setEnabled(mEnabled);
    loadGarden();
    ui->mpGarden->setCurrentIndex(ui->mpGarden->findText(vName));
}

void AddressHomeWidget::saveStreet()
{
    int vCity = ui->mpGarden->itemData(ui->mpGarden->currentIndex()).toInt();

    QString vName = ui->mpStreet->currentText();

    if (vName.isEmpty()) return;
    if (execQuery(QString("SELECT * FROM address_street WHERE name = '%1' AND city_fk = %2")
                  .arg(vName).arg(vCity)).count()) return;

    execQuery(QString("INSERT INTO address_street (name, city_fk) VALUES('%1', %2)")
              .arg(vName)
              .arg(vCity));
    loadStreet();
    ui->mpStreet->setCurrentIndex(ui->mpStreet->findText(vName));
}

void AddressHomeWidget::loadGarden()
{
    ResponseType vResponse = execQuery(QString("select id, name from address_city where type_fk IN (select id from typecity where name='garden')"));
    ui->mpGarden->clear();
    QStringList vList;
    for (int i = 0; i < vResponse.count(); ++i)
    {
        ui->mpGarden->addItem(vResponse[i]["name"].toString(), vResponse[i]["id"]);
        vList << vResponse[i]["name"].toString();
    }


    if (ui->mpGarden->count())
    {
        if (isState(FIND) && !isState(MULTISELECT))
        {
            ui->mpGarden->insertItem(0, "", -1);
            vList.insert(0, "");
        }
        ui->mpGarden->setCurrentIndex(0);
        on_mpGarden_currentIndexChanged(0);
    }
    QCompleter* vpComp = new QCompleter(vList, this);
    vpComp->setCaseSensitivity(Qt::CaseInsensitive);
    ui->mpGarden->setCompleter(vpComp);
}

void AddressHomeWidget::loadStreet()
{
    int vCity = ui->mpGarden->itemData(ui->mpGarden->currentIndex()).toInt();

    ResponseType vResponse = execQuery(QString("select id, name from address_street where city_fk = %1 and microdistrict_fk is null")
                                       .arg(vCity));
    ui->mpStreet->clear();
    QStringList vList;
    for (int i = 0; i < vResponse.count(); ++i)
    {
        ui->mpStreet->addItem(vResponse[i]["name"].toString(), vResponse[i]["id"]);
        vList << vResponse[i]["name"].toString();
    }

    if (ui->mpGarden->currentIndex() >= 0)
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

void AddressHomeWidget::save()
{
    if (ui->mpStreet->currentIndex() < 0) return;
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
    execQuery(QString("UPDATE address SET street_fk = %1, number1 = '%2', number2 = '%3' WHERE id = %4")
              .arg(ui->mpStreet->currentIndex() < 0 ? "NULL" : ui->mpStreet->itemData(ui->mpStreet->currentIndex()).toString())
              .arg(ui->mpNumber1->text())
              .arg(ui->mpNumber2->text())
              .arg(mId));
    MainWidget::save();
}

void AddressHomeWidget::load(int aIdObjects, int aNumber)
{
    MainWidget::load();

    ui->mpNumber1->blockSignals(true);
    ui->mpNumber2->blockSignals(true);
    ui->mpStreet->blockSignals(true);

    mIdObjects = aIdObjects;

    ResponseType vResponseAddress = execQuery(
                QString("SELECT address.id as id, street_fk, address.microdistrict_fk as microdistrict_fk, number1, number2, room, landmark, address_city.id as city_fk FROM address LEFT JOIN address_street ON address.street_fk = address_street.id LEFT JOIN address_microdistrict ON address_microdistrict.id = address.microdistrict_fk LEFT JOIN address_city ON address_city.id = address_street.city_fk OR address_city.id = address_microdistrict.city_fk LEFT JOIN typecity ON typecity.id = address_city.type_fk AND  typecity.name = 'garden' WHERE  object_fk = %1")
                .arg(mIdObjects));
    if (vResponseAddress.count() >= aNumber)
    {
        ResponseRecordType vQuery = vResponseAddress[aNumber - 1];

        ui->mpNumber1->setText(vQuery["number1"].toString());
        ui->mpNumber2->setText(vQuery["number2"].toString());

        ui->mpGarden->setCurrentIndex(ui->mpGarden->findData(vQuery["city_fk"].toInt()));
        ui->mpStreet->setCurrentIndex(ui->mpStreet->findData(vQuery["street_fk"].toInt()));

        mId = vQuery["id"].toInt();
        MainWidget::save();
    }
    else
    {
        ui->mpNumber1->setText("");
        ui->mpNumber2->setText("");

        ui->mpGarden->setCurrentIndex(-1);
        ui->mpStreet->setCurrentIndex(-1);
    }

    ui->mpNumber1->blockSignals(false);
    ui->mpNumber2->blockSignals(false);
    ui->mpStreet->blockSignals(false);

}

void AddressHomeWidget::setEnable(bool aEnable)
{
    ui->mpGarden->setEnabled(aEnable);
    mEnabled = aEnable;
    if (!ui->mpGarden->count()) aEnable = false;
    ui->mpStreet->setEnabled(aEnable);
    if (!ui->mpStreet->count()) aEnable = false;
    ui->mpNumber1->setEnabled(aEnable);
    ui->mpNumber2->setEnabled(aEnable);
}

QList<int> AddressHomeWidget::garden()
{
    if (isState(MULTISELECT))
    {
        return idsComboBox(ui->mpGarden);
    }
    return  idComboBox(ui->mpGarden);
}

QList<int> AddressHomeWidget::street()
{
    if (isState(MULTISELECT))
    {
        return idsComboBox(ui->mpStreet);
    }
    return  idComboBox(ui->mpStreet);
}

int AddressHomeWidget::number1()
{
    return ui->mpNumber1->text().toInt();
}

int AddressHomeWidget::number2()
{
    return ui->mpNumber2->text().toInt();
}

bool AddressHomeWidget::isGarden()
{
    return garden().count() > 0;
}

bool AddressHomeWidget::isStreet()
{
    return street().count() > 0;
}

bool AddressHomeWidget::isNumber1()
{
    return number1() > 0;
}

bool AddressHomeWidget::isNumber2()
{
    return number2() > 0;
}

bool AddressHomeWidget::isState(int aState)
{
    return mState & aState;
}

void AddressHomeWidget::setState(int aState)
{
    mState = aState;
    if (isState(NORMAL))
    {
        ui->mpGarden->setVisible(true);
        ui->mpNumber1->setVisible(true);
        ui->mpNumber2->setVisible(true);
        ui->mpStreet->setVisible(true);
        ui->label->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_4->setVisible(true);
        ui->mpStreet->blockSignals(false);
    }

    if (isState(FIND))
    {
        ui->mpGarden->setVisible(true);
        ui->mpNumber1->setVisible(false);
        ui->mpNumber2->setVisible(false);
        ui->mpStreet->setVisible(true);

        ui->label_3->setVisible(false);
        ui->label_4->setVisible(false);
    }

    if (isState(FINDHOME))
    {
        ui->mpGarden->setVisible(true);
        ui->mpNumber1->setVisible(true);
        ui->mpNumber2->setVisible(true);
        ui->mpStreet->setVisible(true);

        ui->label_3->setVisible(true);
        ui->label_4->setVisible(true);
    }
    if (isState(MULTISELECT))
    {
        connect(&mMapper, SIGNAL(mapped(int)), this, SLOT(selectComboBox(int)));
        for (int i = 0; i < mBoxes.count(); ++i)
        {
            connect(mBoxes[i], SIGNAL(activated(int)), &mMapper, SLOT(map()));
            mMapper.setMapping(mBoxes[i], i);
        }
    }
    loadGarden();
}

void AddressHomeWidget::on_mpGarden_currentIndexChanged(int index)
{
    loadStreet();
    if (index >= 0)
    {
        ui->mpStreet->setEnabled(mEnabled);
    }
    else
    {
        ui->mpStreet->setEnabled(false);
    }
}

void AddressHomeWidget::on_mpStreet_currentIndexChanged(int index)
{
    if (index >= 0)
    {
        setEnabledWidgets(true);
    }
    else
    {
        setEnabledWidgets(false);
    }
}

bool AddressHomeWidget::canSave()
{
    return (ui->mpStreet->currentIndex() >= 0) && (ui->mpNumber1->text().length());
}

QList<int> AddressHomeWidget::idsComboBox(QComboBox* apBox)
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

QList<int> AddressHomeWidget::idComboBox(QComboBox* apBox)
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


void AddressHomeWidget::clickItemComboBox(QComboBox *apBox, int aIndex)
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

 void AddressHomeWidget::selectComboBox(int aIndex)
 {
     clickItemComboBox(mBoxes[aIndex], mBoxes[aIndex]->currentIndex());
 }
