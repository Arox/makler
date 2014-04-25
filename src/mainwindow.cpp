#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogauthentification.h"

#include "globalsbase.h"
#include "dialoguniversal.h"
#include "helpwidget.h"
#include "aboutproducerwidget.h"
#include "dialogasideup.h"
#include "styles.h"
#include "settingswidget.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->mpInformation->setVisible(false);
    mStatus = INIZIALIZE;
    DialogAuthentification v;
    if (v.exec() == QDialog::Rejected) exit(0);
    countMail();
    mUser_fk = execQuery(QString("SELECT mans.id as id FROM mans INNER JOIN users ON users.man_fk = mans.id WHERE users.id = %1").arg(v.idAgent()))[0]["id"].toInt();
    //mUser_fk = v.idAgent();
    mRoles = v.roles();

    DialogAsideUp vAsideUp(mUser_fk, this);
    while (vAsideUp.countAside())
    {
        vAsideUp.exec();
        if (vAsideUp.countAside())
        {
            close();
            if (mStatus == CLOSE) exit(0);
        }
    }

    mStatus = WORK;
    mTimer.setSingleShot(false);
    mTimer.setInterval(5 * 1000);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(countMail()));
    mTimer.start();

    //ui->widget_2->setStyleSheet("border: 0px;");

    QString vStyleMenuButton = "background-color: #dad7d5; border-width: 0px;";
    ui->mpMialView->setStyleSheet(vStyleMenuButton);
    ui->mpInformation->setStyleSheet(vStyleMenuButton);
    ui->mpHelp->setStyleSheet(vStyleMenuButton);

    mpArea = new QScrollArea(ui->centralWidget);
    mpArea->setBackgroundRole(QPalette::NoRole);
    //mpArea->setStyleSheet("background-color: #dad7d5; border-width: 0px;");
    mpArea->setWidget(ui->widget_2);
    mpArea->setWidgetResizable(true);

    ui->horizontalLayout_6->addWidget(mpArea);

    ui->mpStyles->addItems(Styles::styles());
    connect(ui->mpStyles, SIGNAL(activated(QString)), this, SLOT(changeStyle(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *apEvent)
{
    bool vResult = QMessageBox::information(this, TRANSLATE("Выход"), TRANSLATE("Вы действительно хотите выйти из программы?"),QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok;
    if (vResult)
    {
        apEvent->accept();
        mStatus = CLOSE;
    }
    else
    {
        apEvent->ignore();
        mStatus = WORK;
    }
}

bool MainWindow::event(QEvent * event)
{
    return QMainWindow::event(event);
}

int MainWindow::user_fk()
{
    return mUser_fk;
}

QStringList MainWindow::roles() const
{
    return mRoles;
}

void MainWindow::disconnectWidget(int aIndex)
{
    WidgetForControl* vpWidget = mWidgets[aIndex];
    disconnect(vpWidget, SIGNAL(changeWidget(WidgetForControl*)), this, SLOT(load(WidgetForControl*)));
    disconnect(vpWidget, SIGNAL(back()), this, SLOT(backWidget()));
    disconnect(this, SIGNAL(reloadWidget(WidgetForControl*)), vpWidget, SLOT(reload(WidgetForControl*)));
}

void MainWindow::connectWidget(int aIndex)
{
    WidgetForControl* vpWidget = mWidgets[aIndex];
    connect(vpWidget, SIGNAL(changeWidget(WidgetForControl*)), this, SLOT(load(WidgetForControl*)));
    connect(vpWidget, SIGNAL(back()), this, SLOT(backWidget()));
    connect(this, SIGNAL(reloadWidget(WidgetForControl*)), vpWidget, SLOT(reload(WidgetForControl*)));
}

void MainWindow::setSize(int aIndex)
{
    WidgetForControl* vpWidget = mWidgets[aIndex];
    QRect vRect = geometry();
    QDesktopWidget mydesk;


    if (mWidgets.count() == 1)
    {
        showMaximized();
        return;
    }
    return;

    int vNewHeight = vRect.height();
    int vNewWidth = vRect.width();


    int vMaxWidth = mydesk.width();
    int vMaxHeight = mydesk.height();

    QSize vMaxSize = vpWidget->maxSize();
    if (vMaxSize.width() > vMaxWidth) vMaxSize.setWidth(vMaxWidth);
    if (vMaxSize.height() > vMaxHeight) vMaxSize.setHeight(vMaxHeight);

    if (vMaxSize.width()) vMaxWidth = vMaxSize.width();
    if (vMaxSize.height()) vMaxHeight = vMaxSize.height();

    if (vNewHeight < vpWidget->size().height())
    {
        vNewHeight = vpWidget->size().height();
    }
    if (vpWidget->size().height() && (vNewHeight > vpWidget->size().height()))
    {
        vNewHeight = vpWidget->size().height();
    }

    if (vNewWidth < vpWidget->size().width())
    {
        vNewWidth = vpWidget->size().width();
    }
    if (vpWidget->size().width() && (vNewWidth > vpWidget->size().width()))
    {
        vNewWidth = vpWidget->size().width();
    }

    if (mydesk.width() < vNewWidth) vNewWidth = mydesk.width();
    if (mydesk.height() - 300 < vNewHeight) vNewHeight = mydesk.height() - 300;

    vRect.setHeight(vNewHeight);
    vRect.setWidth(vNewWidth);
    //setMinimumSize(vMinWidth, vMinHeight);
    //setMaximumSize(vMaxWidth, vMaxHeight);
    //vpWidget->setMinimumSize(vMinWidth, vMinHeight);
    //vpWidget->setMaximumSize(vMaxWidth, vMaxHeight);

    if (mWidgets.count() == 1)
    {
        //setMaximumSize(vMaxWidth, vMaxHeight);
        //vpWidget->setMaximumSize(vMaxWidth, vMaxHeight);
        showNormal();
    }
    else
    {
        //setMaximumSize(16000, 16000);
        //vpWidget->setMaximumSize(16000, 16000);
        showMaximized();
    }
    vpWidget->setGeometry(vRect);

    resize(vRect.size());

    if (x() < 20) move(20, y());
    if (y() < 20) move(x(), 20);

    move((mydesk.width() - width()) / 2, (mydesk.height() - height()) / 2);


}

void MainWindow::load(WidgetForControl* apControl)
{
    if (mWidgets.count())
    {
        disconnectWidget(mWidgets.count() - 1);
        WidgetForControl* vpWidget = mWidgets[mWidgets.count() - 1];
        ui->mainLayout->removeWidget(vpWidget);
        vpWidget->setParent(0);
    }
    mWidgets.append(apControl);
    connectWidget(mWidgets.count() - 1);
    ui->mainLayout->addWidget(mWidgets[mWidgets.count() - 1]);
    //setWindowTitle(mWidgets[mWidgets.count() - 1]->name());
    //ui->mpNamePage->clear();
    //ui->mpNamePage->setText(mWidgets[mWidgets.count() - 1]->name());
    setSize(mWidgets.count() - 1);
}

void MainWindow::backWidget()
{
    WidgetForControl* vpWidget = 0;
    if (mWidgets.count())
    {
        vpWidget = mWidgets[mWidgets.count() - 1];
        disconnectWidget(mWidgets.count() - 1);
        mWidgets.removeLast();
        vpWidget->setParent(0);
        ui->mainLayout->removeWidget(vpWidget);
        connectWidget(mWidgets.count() - 1);
        emit reloadWidget(vpWidget);
        //delete vpWidget;

        if (!mWidgets.count()) return;

        ui->mainLayout->addWidget(mWidgets[mWidgets.count() - 1]);
        setWindowTitle(mWidgets[mWidgets.count() - 1]->name());
        //ui->mpNamePage->clear();
        //ui->mpNamePage->setText(mWidgets[mWidgets.count() - 1]->name());
        setSize(mWidgets.count() - 1);
    }
}

void MainWindow::closeWidget()
{

}

void MainWindow::countMail()
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
            ui->mpMialView->setText(QString("%1").arg(vCountMail));
            ui->mpMialView->setVisible(true);
        }
        else
        {
            ui->mpMialView->setText("");
            ui->mpMialView->setVisible(false);
        }
    }
}

void MainWindow::on_mpHelp_clicked()
{
    HelpWidget* vWidget = new HelpWidget();
    DialogUniversal vDialog(vWidget, TRANSLATE("Справка"), this);
    vDialog.showMaximized();
    vDialog.exec();
}

void MainWindow::on_mpInformation_clicked()
{
    AboutProducerWidget* vWidget = new AboutProducerWidget();
    DialogUniversal vDialog(vWidget, TRANSLATE("Справка"), this);
    vDialog.exec();
}

void MainWindow::closeWindow()
{
    close();
}

void MainWindow::changeStyle(QString aNameStyle)
{
    Styles::setStyle(aNameStyle);
}

void MainWindow::on_mpSettings_clicked()
{
    load(new SettingsWidget(mUser_fk, mRoles, this));
}
