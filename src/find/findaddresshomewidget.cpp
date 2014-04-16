#include "findaddresshomewidget.h"

FindAddressHomeWidget::FindAddressHomeWidget(QWidget *parent) :
    AddressHomeWidget(FIND|MULTISELECT, parent)
{
}

QString FindAddressHomeWidget::sqlWhere()
{
    QStringList vCity;
    QStringList vStreet;

    foreach (int vId, garden()) {
        vCity << QString("address.city_fk = %1").arg(vId);
    }
    foreach (int vId, street()) {
        vStreet << QString("address.street_fk = %1").arg(vId);
    }
    QStringList vWheres;
    vWheres
            //<< (isGarden() ? QString("address.city_fk = %1").arg(garden()) : QString(""))
            //<< (isStreet() ? QString("address.street_fk = %1").arg(street()) : QString(""))
            << "(" + vCity.join(" OR ") + ")"
            << "(" + vStreet.join(" OR ") + ")"
            << (isNumber1() ? QString("address.number1 = %1").arg(number1()) : QString(""))
            << (isNumber2() ? QString("address.number2 = %1").arg(number2()) : QString(""))
               ;

    vWheres.removeAll(QString(""));
    vWheres.removeAll(QString("()"));
    return vWheres.join(QString(" AND "));
}

QString FindAddressHomeWidget::joinWhere()
{
    return QString("LEFT JOIN address_for_widget ON address_for_widget.object_fk = objects.id INNER JOIN address ON address_for_widget.address1_fk = address.id OR address_for_widget.address2_fk = address.id OR address_for_widget.address3_fk = address.id");
}
