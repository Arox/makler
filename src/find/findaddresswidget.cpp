#include "findaddresswidget.h"

FindAddressWidget::FindAddressWidget(QWidget *parent) :
    AddressWidget(FIND | MULTISELECT, parent)
{
}

QString FindAddressWidget::sqlWhere()
{
    QStringList vCity;
    QStringList vLocation;
    QStringList vStreet;

    foreach (int vId, city()) {
        vCity << QString("address.city_fk = %1").arg(vId);
    }
    foreach (int vId, locality()) {
        vLocation << QString("address.microdistrict_fk = %1").arg(vId);
    }
    foreach (int vId, street()) {
        vStreet << QString("address.street_fk = %1").arg(vId);
    }

    QStringList vWheres;
    vWheres
            //<< (isCity() ? QString("address.city_fk = %1").arg(city()) : QString(""))
            //<< (isLocality() ? QString("address.microdistrict_fk = %1").arg(locality()) : QString(""))
            //<< (isStreet() ? QString("address.street_fk = %1").arg(street()) : QString(""))
            << "(" + vCity.join(" OR ") + ")"
            << "(" + vLocation.join(" OR ") + ")"
            << "(" + vStreet.join(" OR ") + ")"
            << (isNumber1() ? QString("address.number1 = '%1'").arg(number1()) : QString(""))
            << (isNumber2() ? QString("address.number2 = '%1'").arg(number2()) : QString(""))
            << (isRoom() ? QString("address.room = '%1'").arg(room()) : QString(""))
            << (isLandmark() ? QString("address.landmark = '%1'").arg(landmark()) : QString(""));

    vWheres.removeAll(QString(""));
    vWheres.removeAll(QString("()"));
    return QString("(%1)").arg(vWheres.join(QString(" AND ")));
}

QString FindAddressWidget::joinWhere()
{
    return QString("LEFT JOIN address_for_widget ON address_for_widget.object_fk = objects.id INNER JOIN address ON address_for_widget.address1_fk = address.id OR address_for_widget.address2_fk = address.id OR address_for_widget.address3_fk = address.id");
}
