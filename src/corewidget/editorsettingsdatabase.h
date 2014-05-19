#ifndef EDITORSETTINGSDATABASE_H
#define EDITORSETTINGSDATABASE_H

#include <QWidget>
#include "connecttodatabase.h"

namespace Ui {
class EditorSettingsDatabase;
}

class EditorSettingsDatabase : public QWidget
{
    Q_OBJECT

public:
    explicit EditorSettingsDatabase(QWidget *parent = 0);
    ~EditorSettingsDatabase();
    ConnectToDatabase mConnectWidget;


private slots:
    void on_mpSave_clicked();

private:
    Ui::EditorSettingsDatabase *ui;
};

#endif // EDITORSETTINGSDATABASE_H
