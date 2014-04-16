#include "generalwindow.h"
#include "ui_generalwindow.h"

GeneralWindow::GeneralWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GeneralWindow)
{
    ui->setupUi(this);
}

GeneralWindow::~GeneralWindow()
{
    delete ui;
}
