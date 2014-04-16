#include "dialogauthentification.h"
#include "ui_dialogauthentification.h"

#include "globalsbase.h"

#include <QDesktopWidget>
#include <QSqlQuery>
#include <QMessageBox>

#include <QDebug>

DialogAuthentification::DialogAuthentification(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAuthentification)
{
    ui->setupUi(this);
    QDesktopWidget mydesk;
    move(mydesk.width() / 2 - width() / 2, mydesk.height() / 2 - height() / 2);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

    setObjectName("authorization");

    mpLineLogin = new MyLineEdit(TRANSLATE("Логин"), QLineEdit::Normal, this);
    mpLineLogin->setObjectName("login");
    mpLinePassword = new MyLineEdit(TRANSLATE("Пароль"), QLineEdit::Password, this);
    mpLinePassword->setObjectName("password");
    ui->verticalLayout->insertWidget(1, mpLineLogin);
    ui->verticalLayout->insertWidget(2, mpLinePassword);
}

DialogAuthentification::~DialogAuthentification()
{
    delete ui;
}

int DialogAuthentification::idAgent()
{
    return mId;
}

void DialogAuthentification::on_mpOk_clicked()
{
    QStringList vSql;
    vSql << "SELECT users.id as id, roles.id as role_fk, inside_role, display_role"
         << "FROM users"
         << "INNER JOIN users_roles ON users_roles.user_fk = users.id"
         << "INNER JOIN roles ON roles.id = users_roles.role_fk"
         << QString("WHERE login = '%1' AND \"password\" = md5('%2')")
            .arg(mpLineLogin->text())
            .arg(mpLinePassword->text());
            ;
    ResponseType vResponse = execQuery(vSql.join(" "));

    if (vResponse.count())
    {
        QList<int> vRolesId;
        foreach (ResponseRecordType vRecord, vResponse)
        {
            mRoles.append(vRecord["inside_role"].toString());
            vRolesId.append(vRecord["role_fk"].toInt());
        }

        mId = vResponse[0]["id"].toInt();
        accept();
        return;
    }
    QMessageBox::warning(this, TRANSLATE("Ошибка аутентификации"), TRANSLATE("Пользователь не найден"));
}

void DialogAuthentification::on_mpExit_clicked()
{
    reject();
}

QStringList DialogAuthentification::roles() const
{
    return mRoles;
}
