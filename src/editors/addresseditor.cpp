#include "addresseditor.h"
#include "ui_addresseditor.h"
#include "globalsbase.h"
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QCompleter>

AddressEditor::AddressEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddressEditor)
{
    mTypesCitys << "city" << "garden";
    ui->setupUi(this);
    connect(ui->mpCity->lineEdit(), SIGNAL(returnPressed()), this, SLOT(saveCity()));
    connect(ui->mpLocality->lineEdit(), SIGNAL(returnPressed()), this, SLOT(saveLocality()));
    connect(ui->mpStreet->lineEdit(), SIGNAL(returnPressed()), this, SLOT(saveStreet()));

    //ui->mpStreet->blockSignals(true);
    loadCity();
    loadLocality();
    loadStreet();
    //ui->mpStreet->blockSignals(false);

    connect(ui->mpCity, SIGNAL(currentIndexChanged(int)), this, SLOT(on_mpCity_currentIndexChanged(int)));
    //connect(ui->mpLocality, SIGNAL(currentIndexChanged(int)), this, SLOT(on_mpLocality_currentIndexChanged(int)));
}

AddressEditor::~AddressEditor()
{
    delete ui;
}

QString AddressEditor::typeCity() const
{
    return mTypesCitys.at(ui->mpType->currentIndex());
}

void AddressEditor::loadCity()
{
    ui->mpCity->blockSignals(true);
    int vType = execQuery(QString("select id from typecity where name='%1'").arg(typeCity()))[0]["id"].toInt();
    ResponseType vResponse = execQuery(QString("select id, name from address_city where type_fk = %1")
                                       .arg(vType));
    ui->mpCity->clear();
    QStringList vList;
    for (int i = 0; i < vResponse.count(); ++i)
    {
        ui->mpCity->addItem(vResponse[i]["name"].toString(), vResponse[i]["id"]);
        vList << vResponse[i]["name"].toString();
    }
    ui->mpCity->setCompleter(new QCompleter(vList, this));
    if (ui->mpCity->count())
    {
        ui->mpCity->setCurrentIndex(0);
    }
    ui->mpCity->blockSignals(false);
}

void AddressEditor::loadLocality()
{
    ui->mpLocality->blockSignals(true);
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
    ui->mpLocality->setCompleter(new QCompleter(vList, this));

    if (ui->mpCity->count())
    {
        ui->mpLocality->setEnabled(true);
    }
    else
    {
        ui->mpLocality->setEnabled(false);
    }
    if (ui->mpLocality->count())
    {
        ui->mpLocality->setCurrentIndex(0);
    }
    ui->mpLocality->blockSignals(false);
}

void AddressEditor::loadStreet()
{
    ui->mpStreet->blockSignals(true);
    int vCity = ui->mpCity->itemData(ui->mpCity->currentIndex()).toInt();
    //int vLocality = (ui->mpLocality->currentIndex() < 0 ? -1 : ui->mpLocality->itemData(ui->mpLocality->currentIndex()).toInt());

    ResponseType vResponse = execQuery(QString("select id, name from address_street where city_fk = %1")
                                       .arg(vCity)
                                     );
    ui->mpStreet->clear();
    QStringList vList;
    for (int i = 0; i < vResponse.count(); ++i)
    {
        QString s1 = vResponse[i]["name"].toString();
        int s2 = vResponse[i]["id"].toInt();
        ui->mpStreet->addItem(s1, s2);
        vList << s1;
    }
    ui->mpStreet->setCompleter(new QCompleter(vList, this));

    if (ui->mpCity->count())
    {
        ui->mpStreet->setEnabled(true);
    }
    else
    {
        ui->mpStreet->setEnabled(false);
    }

    if (ui->mpStreet->count())
    {
        ui->mpStreet->setCurrentIndex(0);
    }
    ui->mpStreet->blockSignals(false);
}

void AddressEditor::saveCity()
{
    QString vName= ui->mpCity->currentText();
    if (vName.isEmpty()) return;
    int vType = execQuery(QString("select id from typecity where name='%1'").arg(typeCity()))[0]["id"].toInt();
    if (execQuery(QString("SELECT * FROM address_city WHERE name = '%1' and type_fk = %2").arg(vName).arg(vType)).count()) return;


    execQuery(QString("INSERT INTO address_city (name, parent, type_fk) VALUES('%1', NULL, %2)")
              .arg(vName)
              .arg(vType));
    ui->mpLocality->setEnabled(true);
    ui->mpStreet->setEnabled(true);
    loadCity();
    ui->mpCity->setCurrentIndex(ui->mpCity->findText(vName));
}

void AddressEditor::saveLocality()
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

void AddressEditor::saveStreet()
{
    int vCity = ui->mpCity->itemData(ui->mpCity->currentIndex()).toInt();
    QString vName = ui->mpStreet->currentText();

    if (vName.isEmpty()) return;
    if (execQuery(QString("SELECT * FROM address_street WHERE name = '%1' AND city_fk = %2")
                  .arg(vName).arg(vCity)).count()) return;


    execQuery(QString("INSERT INTO address_street (name, city_fk) VALUES('%1', %2)")
              .arg(vName)
              .arg(vCity)
              );
    loadStreet();
    ui->mpStreet->setCurrentIndex(ui->mpStreet->findText(vName));
}

void AddressEditor::on_mpCity_currentIndexChanged(int index)
{
    if (index >= 0)
    {
        ui->mpLocality->setEnabled(true);
        ui->mpStreet->setEnabled(true);
        loadStreet();
        loadLocality();
    }
    else
    {
        ui->mpLocality->setEnabled(false);
        ui->mpStreet->setEnabled(false);
    }
}

/*void AddressEditor::on_mpLocality_currentIndexChanged(int index)
{
    if (index >= 0)
    {
        loadStreet();
    }
}*/

void AddressEditor::on_mpAddCity_clicked()
{
    QString vText = QInputDialog::getText(this, TRANSLATE("Добавление населенного пункта"), TRANSLATE("Введите название населенного пункт"));
    if (vText.length())
    {
        ui->mpCity->setCurrentText(vText);
        saveCity();
    }
}

void AddressEditor::on_mpRenameCity_clicked()
{
    int vIdCity = ui->mpCity->itemData(ui->mpCity->currentIndex()).toInt();
    if (vIdCity < 1) return;
    /*if (execQuery(QString("SELECT * FROM address_street, address_microdistrict WHERE address_street.city_fk = %1 OR address_microdistrict.city_fk = %1").arg(vIdCity)).count())
    {
        QMessageBox::warning(this, TRANSLATE("Нельзя переименовать"), TRANSLATE("Нельзя переименовать, так как есть адреса с этим населенным пунктом."));
        return;
    }*/
    QString vNewName = QInputDialog::getText(this, TRANSLATE("Введите название населеного пункта"), QString(TRANSLATE("Введите новое название для населенного пункта \"%1\"").arg(ui->mpCity->currentText())));
    if (vNewName.length())
    {
        execQuery(QString("UPDATE address_city SET name = '%1' WHERE id = %2")
                  .arg(vNewName)
                  .arg(vIdCity));
    }
    loadCity();
}


void AddressEditor::on_mpRemove_clicked()
{

}

void AddressEditor::on_mpAddLocality_clicked()
{
    int vIdCity = ui->mpCity->itemData(ui->mpCity->currentIndex()).toInt();
    if (vIdCity < 1)
    {
        QMessageBox::warning(this, TRANSLATE("Нельзя добавить"), TRANSLATE("Не выбран населенный пункт"));
        return;
    }
    QString vText = QInputDialog::getText(this, TRANSLATE("Добавление района"), TRANSLATE("Введите название района"));
    if (vText.length())
    {
        ui->mpLocality->setCurrentText(vText);
        saveLocality();
    }
}

void AddressEditor::on_mpRenameLocality_clicked()
{
    int vIdLocality = ui->mpLocality->itemData(ui->mpLocality->currentIndex()).toInt();
    if (vIdLocality < 1) return;

    QString vNewName = QInputDialog::getText(this, TRANSLATE("Введите название района"), QString(TRANSLATE("Введите новое название для района \"%1\"").arg(ui->mpLocality->currentText())));
    if (vNewName.length())
    {
        execQuery(QString("UPDATE address_microdistrict SET name = '%1' WHERE id = %2")
                  .arg(vNewName)
                  .arg(vIdLocality));
    }
    loadLocality();
}

void AddressEditor::on_mpRemoveLocality_clicked()
{
    int vIdLocality = ui->mpLocality->itemData(ui->mpLocality->currentIndex()).toInt();
    if (vIdLocality < 1) return;
    if (execQuery(QString("SELECT * FROM address WHERE address.microdistrict_fk = %1").arg(vIdLocality)).count())
    {
        QMessageBox::warning(this, TRANSLATE("Нельзя удалить"), TRANSLATE("Нельзя удалить, так как есть адреса с этим районом."));
        return;
    }
    execQuery(QString("DELETE address_microdistrict WHERE id = %1")
                  .arg(vIdLocality));
    loadLocality();
}

void AddressEditor::on_mpAddStreet_clicked()
{
    int vIdCity = ui->mpCity->itemData(ui->mpCity->currentIndex()).toInt();
    if (vIdCity < 1)
    {
        QMessageBox::warning(this, TRANSLATE("Нельзя добавить"), TRANSLATE("Не выбран населенный пункт"));
        return;
    }
    QString vText = QInputDialog::getText(this, TRANSLATE("Добавление улицы"), TRANSLATE("Введите название улицы"));
    if (vText.length())
    {
        ui->mpStreet->setCurrentText(vText);
        saveStreet();
    }
    loadStreet();
}

void AddressEditor::on_mpRenameStreet_clicked()
{
    int vIdStreet = ui->mpStreet->itemData(ui->mpStreet->currentIndex()).toInt();
    if (vIdStreet < 1) return;

    QString vNewName = QInputDialog::getText(this, TRANSLATE("Введите название улицы"), QString(TRANSLATE("Введите новое название для улицы \"%1\"").arg(ui->mpStreet->currentText())));
    if (vNewName.length())
    {
        execQuery(QString("UPDATE address_street SET name = '%1' WHERE id = %2")
                  .arg(vNewName)
                  .arg(vIdStreet));
    }
    loadStreet();
}

void AddressEditor::on_mpRemoveStreet_clicked()
{
    int vIdStreet = ui->mpStreet->itemData(ui->mpStreet->currentIndex()).toInt();
    if (vIdStreet < 1) return;
    if (execQuery(QString("SELECT * FROM address WHERE address.street_fk = %1").arg(vIdStreet)).count())
    {
        QMessageBox::warning(this, TRANSLATE("Нельзя удалить"), TRANSLATE("Нельзя удалить, так как есть адреса с этой улицей."));
        return;
    }
    execQuery(QString("DELETE address_street WHERE id = %1")
                  .arg(vIdStreet));
    loadStreet();
}

void AddressEditor::on_mpType_activated(int index)
{
    bool vFlag = true;
    if (index == 1)
    {
        vFlag = false;
        ui->label_7->setText(TRANSLATE("с/т"));
    }
    else
    {
        ui->label_7->setText(TRANSLATE("город"));
    }

    ui->mpLabelLocality->setVisible(vFlag);
    ui->mpLocality->setVisible(vFlag);
    ui->mpAddLocality->setVisible(vFlag);
    ui->mpRenameLocality->setVisible(vFlag);
    ui->mpRemoveLocality->setVisible(vFlag);

    loadCity();

}

void AddressEditor::on_mpRename_clicked()
{

}

void AddressEditor::on_mpRemoveCity_clicked()
{
    int vIdCity = ui->mpCity->itemData(ui->mpCity->currentIndex()).toInt();
    if (vIdCity < 1) return;
    if (execQuery(QString("SELECT * FROM address_street, address_microdistrict WHERE address_street.city_fk = %1 OR address_microdistrict.city_fk = %1").arg(vIdCity)).count())
    {
        QMessageBox::warning(this, TRANSLATE("Нельзя удалить"), TRANSLATE("Нельзя удалить, так как есть адреса с этим населенным пунктом. Удалите сначала все районы и все улицы"));
        return;
    }
    execQuery(QString("DELETE address_city WHERE id = %1")
                  .arg(vIdCity));
    loadCity();
}
