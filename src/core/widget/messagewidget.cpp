#include "messagewidget.h"
#include "ui_messagewidget.h"
#include "language.h"

MessageWidget::MessageWidget(int aUser_fk, QWidget *parent) :
    WidgetForControl(parent),
    ui(new Ui::MessageWidget),
    mUser_fk(aUser_fk),
    mViewPost(aUser_fk)
{
    ui->setupUi(this);
    ui->mainLayout->addWidget(&mViewPost);

    mTimer.setSingleShot(false);
    mTimer.setInterval(5 * 1000);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(countMail()));
    mTimer.start();
}

void MessageWidget::countMail()
{
    QStringList vQuery;
    vQuery << "SELECT count(messages.id) as count FROM messages"
           << "INNER JOIN messages_to ON messages_to.message_fk = messages.id"
           << QString("WHERE \"to\" = %1 AND readed = false").arg(mUser_fk);
    ResponseType vResponse = execQuery(vQuery.join(" "));
    if (vResponse.count())
    {
        int vCountMail = vResponse[0]["count"].toInt();
        if (vCountMail)
        {
            emit changeName(this, TRANSLATE("Сообщения(%1)").arg(vCountMail));
        }
        else
        {
            emit changeName(this, TRANSLATE("Сообщения"));
        }
    }
}

MessageWidget::~MessageWidget()
{
    delete ui;
}

QString MessageWidget::name()
{
    return TRANSLATE("Сообщения");
}

QSize MessageWidget::minSize()
{
    return QSize(0, 0);
}

QSize MessageWidget::size()
{
    return QSize(787, 800);
}

QSize MessageWidget::maxSize()
{
    return QSize(0, 0);
}

QString MessageWidget::idName()
{
    return "messages";
}
