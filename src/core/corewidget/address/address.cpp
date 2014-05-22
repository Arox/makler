#include "address.h"
#include "ui_address.h"

Address::Address(int aType, int aState, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Address)
  ,mpAddressInCity(0)
  ,mpAddressInGarden(0)
{
    ui->setupUi(this);
    setType(aType, aState);
}

Address::~Address()
{
    delete ui;
}

void Address::setType(int aType, int aState)
{
    if (aType & CITY)
    {
        if (mpAddressInCity) {delete mpAddressInCity;}
        mpAddressInCity =new AddressWidget(aState, this);
        ui->mpCityLayout->addWidget(mpAddressInCity);
    }

    if (aType & GARDEN)
    {
        if (mpAddressInGarden) {delete mpAddressInGarden;}
        mpAddressInGarden =new AddressHomeWidget(aState, this);
        ui->mpGardenLayout->addWidget(mpAddressInGarden);
    }

    if (!(mpAddressInCity && mpAddressInGarden))
    {
        ui->mpCity->setVisible(false);
        ui->mpGarden->setVisible(false);
    }
}


void Address::load(int aIdObjects, int aNumber)
{
    if (mpAddressInCity)
    {
        mpAddressInCity->load(aIdObjects, aNumber);
    }
    if (mpAddressInGarden)
    {
        mpAddressInGarden->load(aIdObjects, aNumber);
    }
}

bool Address::canSave()
{
    return (mpAddressInCity && mpAddressInCity->canSave()) || (mpAddressInGarden && mpAddressInGarden->canSave());
}

void Address::save()
{
    if (mpAddressInCity)
    {
        mpAddressInCity->save();
    }
    if (mpAddressInGarden)
    {
        mpAddressInGarden->save();
    }
}
