#include "addresscity.h"
#include "citymodel.h"

AddressCity::AddressCity(int aState, QWidget *parent) :
    BaseAddressWidget(CityModel::City, aState, parent)
{
}
