#include "agentwidget.h"
#include "ui_agentwidget.h"

#include "globalsbase.h"
#include <QMessageBox>

AgentWidget::AgentWidget(QWidget *parent) :
    MainWidget(parent),
    ui(new Ui::AgentWidget),
    mId(-1)
{
    ui->setupUi(this);

    ResponseType vResponse = execQuery(QString("SELECT id, display_role FROM roles"));
    for (int i = 0; i < vResponse.count(); ++i)
    {
        ui->mpRoles->addItem(vResponse[i]["display_role"].toString());
        mRolesIds.append(vResponse[i]["id"].toInt());
    }

    connect(ui->mpSave, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->mpBack, SIGNAL(clicked()), this, SIGNAL(back()));
}

AgentWidget::~AgentWidget()
{
    delete ui;
}

void AgentWidget::load(int aIdObjects)
{
    MainWidget::load();
    mId = aIdObjects;
    ResponseType vResponse = execQuery(
                QString("SELECT users.login as login, mans.name as name, mans.sername as sername, mans.patronymic as patronymic, roles.id as role, users.is_active as active FROM users INNER JOIN mans ON users.man_fk = mans.id LEFT JOIN users_roles ON users_roles.user_fk = users.id LEFT JOIN roles ON users_roles.role_fk = roles.id WHERE man_fk = %1")
                .arg(mId));

    if (vResponse.count())
    {
        ResponseRecordType vQuery = vResponse[0];

        ui->mpLogin->setText(vQuery["login"].toString());
        ui->mpName->setText(vQuery["name"].toString());
        ui->mpPatronymic->setText(vQuery["patronymic"].toString());
        ui->mpSername->setText(vQuery["sername"].toString());
        ui->mpPassword1->setText("");
        ui->mpPassword2->setText("");

        ui->mpActive->setChecked(vQuery["active"].toBool());

        for (int i = 0; i < vResponse.count(); ++i)
        {
            if (vResponse[i]["role"].isNull()) continue;
            ui->mpRoles->item(mRolesIds.indexOf(vResponse[i]["role"].toInt()))->setSelected(true);
        }
        MainWidget::save();
    }
    else
    {
        mId = -1;
        ui->mpLogin->setText("");
        ui->mpName->setText("");
        ui->mpPatronymic->setText("");
        ui->mpSername->setText("");
        ui->mpPassword1->setText("");
        ui->mpPassword2->setText("");
        ui->mpRoles->clearSelection();
    }

}

void AgentWidget::save()
{
    if (ui->mpName->text().isEmpty())
    {
        QMessageBox::warning(this, TRANSLATE("Ошибка"), TRANSLATE("Поле \"имя\" не может быть пустым"));
        return;
    }
    if (ui->mpSername->text().isEmpty())
    {
        QMessageBox::warning(this, TRANSLATE("Ошибка"), TRANSLATE("Поле \"фамилия\" не может быть пустым"));
        return;
    }
    if (ui->mpPassword1->text() != ui->mpPassword2->text())
    {
        QMessageBox::warning(this, TRANSLATE("Ошибка"), TRANSLATE("Пароль и его подтверждение не совпадают"));
        return;
    }

    if (execQuery(QString("SELECT id FROM users WHERE login = '%1'").arg(ui->mpLogin->text())).count())
    {
        QMessageBox::warning(this, TRANSLATE("Ошибка"), TRANSLATE("Пользователь с таим логином уже существует"));
        return;
    }

    if (mId < 0)
    {
        execQuery(QString("INSERT INTO mans (name, sername, patronymic) VALUES ('%1', '%2', '%3')")
                  .arg(ui->mpName->text())
                  .arg(ui->mpSername->text())
                  .arg(ui->mpPatronymic->text()));
        ResponseType vResponse = execQuery(QString("SELECT max(id) as id FROM mans WHERE name='%1' AND sername = '%2' AND patronymic = '%3'")
                              .arg(ui->mpName->text())
                              .arg(ui->mpSername->text())
                              .arg(ui->mpPatronymic->text()));
        if (vResponse.count())
        {
            mId = vResponse[0]["id"].toInt();
        }
        else
        {
            QMessageBox::warning(this, TRANSLATE("Ошибка"), TRANSLATE("Не удалось сохранить данные"));
            return;
        }

    }
    int vIdUser;
    ResponseType vResponse = execQuery(QString("SELECT id as id from users WHERE man_fk = %1").arg(mId));
    if (!vResponse.count())
    {

        if (!execQuery(QString("INSERT INTO users (login, password, man_fk) VALUES ('%1', md5('%2'), %3)")
                  .arg(ui->mpLogin->text())
                  .arg(ui->mpPassword1->text())
                  .arg(mId)).count())
        {
            QMessageBox::warning(this, TRANSLATE("Ошибка"), TRANSLATE("Не удалось сохранить данные"));
            return;
        }

        vIdUser = execQuery(QString("SELECT id FROM users WHERE login = '%1'").arg(ui->mpLogin->text()))[0]["id"].toInt();

    }
    else
    {
        vIdUser = vResponse[0]["id"].toInt();
    }

    int vIdMan = mId;

    execQuery(QString("UPDATE mans SET name = '%1', sername = '%2', patronymic = '%3' WHERE id = %4")
              .arg(ui->mpName->text())
              .arg(ui->mpSername->text())
              .arg(ui->mpPatronymic->text())
              .arg(vIdMan)
              );
    execQuery(QString("UPDATE users SET login = '%1', is_active = %2 WHERE id = %3")
              .arg(ui->mpLogin->text())
              .arg(ui->mpActive->isChecked() ? "TRUE" : "FALSE")
              .arg(vIdUser));
    if (!ui->mpPassword1->text().isEmpty())
    {
        execQuery(QString("UPDATE users SET password = md5('%1') WHERE id = %2")
                  .arg(ui->mpPassword1->text())
                  .arg(vIdUser));
    }
    execQuery(QString("DELETE FROM users_roles WHERE user_fk = %1").arg(vIdUser));
    for (int i = 0; i < ui->mpRoles->count(); ++i)
    {
        if (ui->mpRoles->item(i)->isSelected())
        {
            execQuery(QString("INSERT INTO users_roles (user_fk, role_fk) VALUES(%1, %2)")
                      .arg(vIdUser)
                      .arg(mRolesIds.at(i)));
        }
    }
    MainWidget::save();
    emit back();
}
