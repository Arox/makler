#include "typehomewidget.h"
#include "ui_typehomewidget.h"

#include "globalsbase.h"

TypeHomeWidget::TypeHomeWidget(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::TypeHomeWidget),
    mId(-1),
    mIdObjects(-1)
  //,mInfoArea(this)
{
    ui->setupUi(this);
    //ui->OneLayout->addWidget(&mInfoArea);
    QStringList vDicts;
    vDicts << "material" << "hometype" << "category";

    QList<QComboBox*> vBoxes;
    vBoxes << ui->mpMaterial << ui->mpFund << ui->mpCategory;
    for (int i =0; i < vDicts.count(); ++i)
    {
         int vDictId = execQuery(
                    QString("SELECT id FROM static_dictionaries WHERE parent is NULL AND \"name\" = '%1'")
                     .arg(vDicts.at(i)))[0]["id"].toInt();
        ResponseType vQuery = execQuery(
                    QString("SELECT id, \"name\" FROM static_dictionaries WHERE parent = %1")
                    .arg(vDictId));
        ResponseRecordType vRecord;
        vBoxes[i]->addItem(TRANSLATE("не выбрано"), -1);
        foreach (vRecord, vQuery)
        {
            vBoxes[i]->addItem(vRecord["name"].toString(), vRecord["id"].toInt());
        }
    }

    /*connect(ui->mpFund, SIGNAL(currentIndexChanged(int)) , this, SLOT(save()));
    connect(ui->mpMaterial, SIGNAL(currentIndexChanged(int)) , this, SLOT(save()));
    connect(ui->mpLevels, SIGNAL(textChanged(QString)), this, SLOT(save()));
    connect(ui->mpArea, SIGNAL(textChanged(QString)), this, SLOT(save()));
    connect(ui->mpBook, SIGNAL(stateChanged(int)), this, SLOT(save()));
    connect(ui->mpRegion, SIGNAL(stateChanged(int)), this, SLOT(save()));
    connect(ui->mpRoomes, SIGNAL(textChanged(QString)), this, SLOT(save()));
    connect(ui->mpSizeX, SIGNAL(textChanged(QString)), this, SLOT(save()));
    connect(ui->mpSizeY, SIGNAL(textChanged(QString)), this, SLOT(save()));
    connect(ui->mpStage, SIGNAL(textChanged(QString)), this, SLOT(save()));
    connect(ui->mpYear, SIGNAL(textChanged(QString)), this, SLOT(save()));*/
}

TypeHomeWidget::~TypeHomeWidget()
{
    delete ui;
}

void TypeHomeWidget::load(int aIdObjects)
{
    mIdObjects = aIdObjects;
    MainWidget::load();
    //mInfoArea.load(aIdObjects);
    ResponseType vResponse = execQuery(
                QString("SELECT id, material_fk, fund_fk, category_fk, levels, area, x, y, stage, rooms, book, region, home, gardenhome, year FROM typehome WHERE object_fk = %1")
                .arg(mIdObjects));

    if (vResponse.count())
    {
        ResponseRecordType vQuery = vResponse[0];
        mId = vQuery["id"].toInt();
        QStringList vFields;
        vFields << "material_fk" << "fund_fk" << "category_fk";
        QList<QComboBox*> vBoxes;
        vBoxes <<  ui->mpMaterial << ui->mpFund << ui->mpCategory;
        for (int i = 0; i < vFields.count(); ++i)
        {
            if (!vQuery[vFields[i]].isNull())
            {
                for (int j = 0; j < vBoxes[i]->count(); ++j)
                {
                    if (vBoxes[i]->itemData(j) == vQuery[vFields[i]])
                    {
                        vBoxes[i]->setCurrentIndex(j);
                        break;
                    }
                }
            }
        }
        ui->mpArea->setText(vQuery["area"].toString());
        ui->mpBook->setChecked(vQuery["book"].toBool());
        ui->mpHome->setChecked(vQuery["home"].toBool());
        ui->mpGardenHome->setChecked(vQuery["gardenhome"].toBool());
        ui->mpLevels->setText(vQuery["levels"].toString());
        ui->mpRoomes->setText(vQuery["rooms"].toString());
        ui->mpRegion->setChecked(vQuery["region"].toBool());
        ui->mpSizeX->setText(vQuery["x"].toString());
        ui->mpSizeY->setText(vQuery["y"].toString());
        ui->mpStage->setText(vQuery["stage"].toString());
        ui->mpYear->setText(vQuery["year"].toString());
        MainWidget::save();
    }
    else
    {
        ui->mpMaterial->setCurrentIndex(0);
        ui->mpFund->setCurrentIndex(0);
        ui->mpArea->setText("");
        ui->mpBook->setChecked(false);
        ui->mpLevels->setText("");
        ui->mpRoomes->setText("");
        ui->mpRegion->setChecked(false);
        ui->mpSizeX->setText("");
        ui->mpSizeY->setText("");
        ui->mpStage->setText("");
        ui->mpYear->setText("");
    }
}

void TypeHomeWidget::save()
{
    //mInfoArea.save();
    if (mId < 0)
    {
        execQuery(QString("INSERT INTO typehome (object_fk) VALUES (%1)")
                  .arg(mIdObjects));
        mId = execQuery(
                    QString("SELECT id FROM typehome WHERE object_fk = %1")
                    .arg(mIdObjects))[0]["id"].toInt();
    }
    execQuery(QString("UPDATE typehome SET material_fk=%1, fund_fk=%2, category_fk=%3, levels=%4, area=%5, x=%6, y=%7, stage=%8, rooms=%9, book=%10, region=%11, home=%12, gardenhome=%13, year=%14 WHERE id = %15")
              .arg(ui->mpMaterial->currentIndex() > 0 ? ui->mpMaterial->itemData(ui->mpMaterial->currentIndex()).toString() : "NULL")
              .arg(ui->mpFund->currentIndex() > 0 ? ui->mpFund->itemData(ui->mpFund->currentIndex()).toString() : "NULL")
              .arg(ui->mpCategory->currentIndex() > 0 ? ui->mpCategory->itemData(ui->mpCategory->currentIndex()).toString() : "NULL")
              .arg(ui->mpLevels->text().isEmpty() ? "0" : ui->mpLevels->text())
              .arg(ui->mpArea->text().isEmpty() ? "0" : ui->mpArea->text())
              .arg(ui->mpSizeX->text().isEmpty() ? "0" : ui->mpSizeX->text())
              .arg(ui->mpSizeY->text().isEmpty() ? "0" : ui->mpSizeY->text())
              .arg(ui->mpStage->text().isEmpty() ? "0" : ui->mpStage->text())
              .arg(ui->mpRoomes->text().isEmpty() ? "0" : ui->mpRoomes->text())
              .arg(ui->mpBook->isChecked() ? "TRUE" : "FALSE")
              .arg(ui->mpRegion->isChecked() ? "TRUE" : "FALSE")
              .arg(ui->mpHome->isChecked() ? "TRUE" : "FALSE")
              .arg(ui->mpGardenHome->isChecked() ? "TRUE" : "FALSE")
              .arg(ui->mpYear->text().isEmpty() ? "0" : ui->mpYear->text())
              .arg(mId));
    MainWidget::save();
}

void TypeHomeWidget::setEnable(bool aEnable)
{
    ui->mpFund->setEnabled(aEnable);
    ui->mpCategory->setEnabled(aEnable);
    ui->mpLevels->setEnabled(aEnable);
    ui->mpMaterial->setEnabled(aEnable);
    ui->mpRoomes->setEnabled(aEnable);
    ui->mpArea->setEnabled(aEnable);
    ui->mpBook->setEnabled(aEnable);
    ui->mpRegion->setEnabled(aEnable);
    ui->mpHome->setEnabled(aEnable);
    ui->mpGardenHome->setEnabled(aEnable);
    ui->mpSizeX->setEnabled(aEnable);
    ui->mpSizeY->setEnabled(aEnable);
    ui->mpStage->setEnabled(aEnable);
    ui->mpYear->setEnabled(aEnable);
    //mInfoArea.setEnable(aEnable);
}
