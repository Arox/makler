#ifndef APPENDDATABASE_H
#define APPENDDATABASE_H

#include <QWidget>
#include "mainwidget.h"

namespace Ui {
class AppendDatabase;
}

class AppendDatabase : public MainWidget
{
    Q_OBJECT

public:
    explicit AppendDatabase(QWidget *parent = 0);
    ~AppendDatabase();

private slots:
    void on_mpButtons_accepted();

    void on_mpButtons_rejected();

private:
    Ui::AppendDatabase *ui;
};

#endif // APPENDDATABASE_H
