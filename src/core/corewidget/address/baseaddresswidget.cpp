#include "baseaddresswidget.h"
#include "ui_addresswidget.h"

#include <QMessageBox>
#include <QTime>
#include <QCompleter>
#include "../general/globalsbase.h"

#include "microdistrictwidget.h"
#include "streetwidget.h"
#include "citywidget.h"

BaseAddressWidget::BaseAddressWidget(CityModel::TypeCity aType, int aState, QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::AddressWidget),
    mId(-1),
    mEnabled(true),
    mState(aState)
  ,mCityBox(tr("город"), new CityWidget(aType, this), this)
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

BaseAddressWidget::~BaseAddressWidget()
{
    delete ui;
}

void BaseAddressWidget::setEnabledWidgets(bool aEnable)
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

bool BaseAddressWidget::isState(int aState)
{
    return mState & aState;
}

void BaseAddressWidget::load(int aIdObjects)
{
    MainWidget::load();

    mIdObjects = aIdObjects;

    QString vType ="city";
    CityWidget* vpCity = dynamic_cast<CityWidget*>(mCityBox.data());
    if (vpCity)
    {
        vType = vpCity->type() == CityModel::City ? "city" : "garden";
    }

    ResponseType vResponseAddress = execQuery(
                QString("SELECT address.id as id, street_fk, address.microdistrict_fk as microdistrict_fk, number1, number2, room, landmark, address_city.id as city_fk FROM address LEFT JOIN address_street ON address.street_fk = address_street.id LEFT JOIN address_microdistrict ON address_microdistrict.id = address.microdistrict_fk LEFT JOIN address_city ON address_city.id = address_street.city_fk OR address_city.id = address_microdistrict.city_fk LEFT JOIN typecity ON typecity.id = address_city.type_fk AND  typecity.name = '%2' WHERE  object_fk = %1")
                .arg(mIdObjects)
                .arg(vType));
    if (vResponseAddress.count())
    {
        ResponseRecordType vQuery = vResponseAddress[0];

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
}

void BaseAddressWidget::save()
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

QList<int> BaseAddressWidget::city()
{
    return mCityBox.data()->values();}

QList<int> BaseAddressWidget::locality()
{
    return mMicrodistrictBox.data()->values();
}

QList<int> BaseAddressWidget::street()
{
    return mStreetBox.data()->values();
}

int BaseAddressWidget::number1()
{
    return ui->mpNumber1->text().toInt();
}

int BaseAddressWidget::number2()
{
    return ui->mpNumber2->text().toInt();
}

int BaseAddressWidget::room()
{
    return ui->mpRoom->text().toInt();
}

QString BaseAddressWidget::landmark()
{
    return ui->mpLandMark->text();
}

bool BaseAddressWidget::isCity()
{
    return city().count() > 0;
}

bool BaseAddressWidget::isLocality()
{
    return locality().count() > 0;
}

bool BaseAddressWidget::isStreet()
{

    return street().count() > 0;
}

bool BaseAddressWidget::isNumber1()
{
    return number1() > 0;
}

bool BaseAddressWidget::isNumber2()
{
    return number2() > 0;
}

bool BaseAddressWidget::isRoom()
{
    return room() > 0;
}

bool BaseAddressWidget::isLandmark()
{
    return !landmark().isEmpty();
}

void BaseAddressWidget::setNormalVisible()
{
    mCityBox.setVisible(true);
    mStreetBox.setVisible(true);
    ui->mpHomeNumberContainer->setVisible(true);
    ui->mpLandMark->setVisible(true);

    CityWidget* vpCityWidget = dynamic_cast<CityWidget*>(mCityBox.data());
    if (vpCityWidget)
    {
        switch (vpCityWidget->type()) {
        case CityModel::City:
                mMicrodistrictBox.setVisible(true);
                ui->mpRoomContainer->setVisible(true);
            break;
        case CityModel::Garden:
                mMicrodistrictBox.setVisible(false);
                ui->mpRoomContainer->setVisible(false);
            break;
        default:
            break;
        }
    }
}

void BaseAddressWidget::setState(int aState)
{
    mState = aState;

    mCityBox.data()->setType(aState);
    mMicrodistrictBox.data()->setType(aState);
    mStreetBox.data()->setType(aState);

    if (isState(location::NORMAL))
    {
        setNormalVisible();
    }
    if (isState(location::FIND))
    {
        setNormalVisible();
        ui->mpLandMark->setVisible(false);

        mCityBox.data()->setEditable(true);
        mStreetBox.data()->setEditable(true);
        mMicrodistrictBox.data()->setEditable(true);

        if (isState(location::FINDROOM))
        {
            ui->mpRoomContainer->setVisible(true);
        }
        else
        {
            ui->mpRoomContainer->setVisible(false);
        }
    }
}

void BaseAddressWidget::on_mpCity_currentIndexChanged(int index)
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

bool BaseAddressWidget::canSave()
{
    return (mStreetBox.data()->currentIndex() >= 0) && (ui->mpNumber1->text().length());
}

void BaseAddressWidget::selectComboBox(int aIndex)
{
    mBoxes[aIndex]->clickItemComboBox(mBoxes[aIndex]->currentIndex());
}
