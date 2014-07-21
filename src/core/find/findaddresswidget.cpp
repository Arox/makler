#include "findaddresswidget.h"

FindAddressWidget::FindAddressWidget(QWidget *parent) :
    BaseAddressWidget(CityModel::City, location::FIND | location::MULTISELECT, parent)
{
}

QString FindAddressWidget::sqlWhere()
{
    QStringList vCity;
    QStringList vLocation;
    QStringList vStreet;

    foreach (int vId, city()) {
        vCity << QString("city_fk = %1").arg(vId);
    }
    foreach (int vId, locality()) {
        vLocation << QString("microdistrict_fk = %1").arg(vId);
    }
    foreach (int vId, street()) {
        vStreet << QString("street_fk = %1").arg(vId);
    }

    QStringList vWheres;
    vWheres
            << "(" + vCity.join(" OR ") + ")"
            << "(" + vLocation.join(" OR ") + ")"
            << "(" + vStreet.join(" OR ") + ")"
            << (isNumber1() ? QString("number1 = '%1'").arg(number1()) : QString(""))
            << (isNumber2() ? QString("number2 = '%1'").arg(number2()) : QString(""))
            << (isRoom() ? QString("room = '%1'").arg(room()) : QString(""))
            << (isLandmark() ? QString("landmark = '%1'").arg(landmark()) : QString(""));

    vWheres.removeAll(QString(""));
    vWheres.removeAll(QString("()"));
    return QString("(%1)").arg(vWheres.join(QString(" AND ")));
}

QString FindAddressWidget::joinWhere()
{
    return QString("LEFT JOIN address_for_widget ON address_for_widget.object_fk = objects.id INNER JOIN address ON address_for_widget.address1_fk = address.id OR address_for_widget.address2_fk = address.id OR address_for_widget.address3_fk = address.id");
}
