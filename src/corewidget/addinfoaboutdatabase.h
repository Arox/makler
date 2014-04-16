#ifndef ADDINFOABOUTDATABASE_H
#define ADDINFOABOUTDATABASE_H

#include <QWidget>

namespace Ui {
class AddInfoAboutDatabase;
}

class AddInfoAboutDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit AddInfoAboutDatabase(QWidget *parent = 0);
    ~AddInfoAboutDatabase();

private:
    Ui::AddInfoAboutDatabase *ui;
};

#endif // ADDINFOABOUTDATABASE_H
