#include "addresswidget.h"
#include "ui_addresswidget.h"

#include <QMessageBox>
#include <QTime>
#include <QCompleter>
#include "../general/globalsbase.h"

#include "microdistrictwidget.h"
#include "streetwidget.h"
#include "citywidget.h"

AddressWidget::AddressWidget(int aState, QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::AddressWidget),
    mId(-1),
    mEnabled(true),
    mState(aState)
  ,mCityBox(tr("город"), new CityWidget(CityModel::City, this), this)
  ,mMicrodistrictBox(tr("район"), new MicrodistrictWidget(this), this)
  ,mStreetBox(tr("улица"), new StreetWidget(this), this)
{
    ui->setupUi(this);
    connect(mCityBox.data(), SIGNAL(currentIndexChanged(int)), this, SLOT(on_mpCity_currentIndexChanged(int)));
    mBoxes << mCityBox.data() << mMicrodistrictBox.data() << mStreetBox.data();
    setState(aState);

    ui->horizontalLayout_3->insertWidget(0,&mCityBox);
    ui->horizontalLayout_3->insertWidget(1,&mMicrodistrictBox);
    ui->horizontalLayout_3->insertWidget(2,&mStreetBox);

    on_mpCity_currentIndexChanged(mCityBox.data()->currentIndex());
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

/*void AddressWidget::saveCity()
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
}*/

bool AddressWidget::isState(int aState)
{
    return mState & aState;
}

void AddressWidget::load(int aIdObjects, int aNumber)
{
    MainWidget::load();

    /*ui->mpLandMark->blockSignals(true);
    ui->mpNumber1->blockSignals(true);
    ui->mpNumber2->blockSignals(true);
    ui->mpRoom->blockSignals(true);
    ui->mpStreet->blockSignals(true);
    ui->mpLocality->blockSignals(true);
*/
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

        if (!vQuery["city_fk"].isNull()) mCityBox.data()->setCurrentIndex(mCityBox.data()->findData(vQuery["city_fk"].toInt())); else mCityBox.data()->setCurrentIndex(-1);
        if (!vQuery["microdistrict_fk"].isNull()) mMicrodistrictBox.data()->setCurrentIndex(mMicrodistrictBox.data()->findData(vQuery["microdistrict_fk"].toInt())); else mMicrodistrictBox.data()->setCurrentIndex(-1);
        if (!vQuery["street_fk"].isNull()) mStreetBox.data()->setCurrentIndex(mStreetBox.data()->findData(vQuery["street_fk"].toInt())); else mStreetBox.data()->setCurrentIndex(mStreetBox.data()->findData(-1));

        MainWidget::save();
    }
    else
    {
        ui->mpLandMark->setText("");
        ui->mpNumber1->setText("");
        ui->mpNumber2->setText("");
        ui->mpRoom->setText("");

        mCityBox.data()->setCurrentIndex(-1);
        mMicrodistrictBox.data()->setCurrentIndex(-1);
        mStreetBox.data()->setCurrentIndex(-1);
    }

/*    ui->mpLandMark->blockSignals(false);
    ui->mpNumber1->blockSignals(false);
    ui->mpNumber2->blockSignals(false);
    ui->mpRoom->blockSignals(false);

    ui->mpStreet->blockSignals(false);
    ui->mpLocality->blockSignals(false);*/
}

void AddressWidget::save()
{
    if (mStreetBox.data()->currentIndex() < 0)
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
              .arg(mStreetBox.data()->currentIndex() < 0 ? "NULL" : mStreetBox.data()->itemData(mStreetBox.data()->currentIndex()).toString())
              .arg(mMicrodistrictBox.data()->currentIndex() < 0 ? "NULL" : mMicrodistrictBox.data()->itemData(mMicrodistrictBox.data()->currentIndex()).toString())
              .arg(ui->mpNumber1->text())
              .arg(ui->mpNumber2->text())
              .arg(ui->mpRoom->text().isEmpty() ? "NULL" : ui->mpRoom->text())
              .arg(ui->mpLandMark->text())
              .arg(mId));
    MainWidget::save();
}

QList<int> AddressWidget::city()
{
    return mCityBox.data()->values();}

QList<int> AddressWidget::locality()
{
    return mMicrodistrictBox.data()->values();
}

QList<int> AddressWidget::street()
{
    return mStreetBox.data()->values();
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

    mCityBox.data()->setType(aState);
    mMicrodistrictBox.data()->setType(aState);
    mStreetBox.data()->setType(aState);

    if (isState(location::NORMAL))
    {
        mCityBox.setVisible(true);
        ui->mpLandMark->setVisible(true);
        mMicrodistrictBox.setVisible(true);
        ui->mpNumber1->setVisible(true);
        ui->mpNumber2->setVisible(true);
        ui->mpRoom->setVisible(true);
        mStreetBox.setVisible(true);
        ui->label->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_6->setVisible(true);
    }
    if (isState(location::FIND))
    {
        mCityBox.setVisible(true);
        ui->mpLandMark->setVisible(false);
        mMicrodistrictBox.setVisible(true);
        mStreetBox.setVisible(true);


        mCityBox.data()->setEditable(true);
        mStreetBox.data()->setEditable(true);
        mMicrodistrictBox.data()->setEditable(true);

        if (isState(location::FINDROOM))
        {
            ui->label_2->setVisible(true);
            ui->label_3->setVisible(true);
            ui->label_6->setVisible(true);
            ui->mpNumber1->setVisible(true);
            ui->mpNumber2->setVisible(true);
            ui->mpRoom->setVisible(true);
            ui->label->setVisible(true);
        }
        else
        {
            ui->label_2->setVisible(false);
            ui->label_3->setVisible(false);
            ui->label_6->setVisible(false);
            ui->mpNumber1->setVisible(false);
            ui->mpNumber2->setVisible(false);
            ui->mpRoom->setVisible(false);
            ui->label->setVisible(false);
        }
    }
}

void AddressWidget::on_mpCity_currentIndexChanged(int index)
{
    if (index >= 0)
    {
        mMicrodistrictBox.data()->setEnabled(mEnabled);
        mStreetBox.data()->setEnabled(mEnabled);

        MicrodistrictWidget* vpMicrodistrict = dynamic_cast<MicrodistrictWidget*>(mMicrodistrictBox.data());
        if (vpMicrodistrict) vpMicrodistrict->load(mCityBox.data()->itemData(mCityBox.data()->currentIndex()).toInt());

        StreetWidget* vpStreet = dynamic_cast<StreetWidget*>(mStreetBox.data());
        if (vpStreet) vpStreet->load(mCityBox.data()->itemData(mCityBox.data()->currentIndex()).toInt());
    }
    else
    {
        mMicrodistrictBox.data()->setEnabled(false);
        mStreetBox.data()->setEnabled(false);
    }
}

bool AddressWidget::canSave()
{
    return (mStreetBox.data()->currentIndex() >= 0) && (ui->mpNumber1->text().length());
}

void AddressWidget::selectComboBox(int aIndex)
{
    mBoxes[aIndex]->clickItemComboBox(mBoxes[aIndex]->currentIndex());
}
