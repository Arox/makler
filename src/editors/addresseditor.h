#ifndef ADDRESSEDITOR_H
#define ADDRESSEDITOR_H

#include <QWidget>

namespace Ui {
class AddressEditor;
}

class AddressEditor : public QWidget
{
    Q_OBJECT
    enum TypeCity{
        CITY = 0,
        GARDEN = 1
    };

public:
    explicit AddressEditor(QWidget *parent = 0);
    ~AddressEditor();
public slots:
    void loadCity();
    void loadLocality();
    void loadStreet();
    void saveCity();
    void saveLocality();
    void saveStreet();
    void on_mpCity_currentIndexChanged(int index);
    //void on_mpLocality_currentIndexChanged(int index);
private slots:
    void on_mpAddCity_clicked();
    
    void on_mpRenameCity_clicked();
    
    void on_mpRemove_clicked();

    void on_mpAddLocality_clicked();

    void on_mpRenameLocality_clicked();

    void on_mpRemoveLocality_clicked();

    void on_mpAddStreet_clicked();

    void on_mpRenameStreet_clicked();

    void on_mpRemoveStreet_clicked();

    void on_mpType_activated(int index);

    void on_mpRename_clicked();

    void on_mpRemoveCity_clicked();

private:
    QString typeCity() const;
private:
    Ui::AddressEditor *ui;
    QStringList mTypesCitys;
};

#endif // ADDRESSEDITOR_H
