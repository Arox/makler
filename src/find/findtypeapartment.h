#ifndef FINDTYPEAPARTMENT_H
#define FINDTYPEAPARTMENT_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include <QSignalMapper>
namespace Ui {
class FindTypeApartment;
}

class FindTypeApartment : public QWidget
{
    Q_OBJECT
    
public:
    explicit FindTypeApartment(QWidget *parent = 0);
    ~FindTypeApartment();
    //int minRooms();
    //int maxRooms();
    int minLevel();
    int maxLevel();
    int minLevels();
    int maxLevels();
    QList<int> material();
    QList<int> apartment();
    QList<int> fund();

    QString sqlWhere();
    QString joinWhere();
protected:
    QList<int> idsComboBox(QComboBox* apBox);
    void clickItemComboBox(QComboBox* apBox, int aIndex);
protected slots:
    void selectComboBox(int aIndex);
private:
    Ui::FindTypeApartment *ui;
    QSignalMapper mMapper;
    QList<QComboBox*> mBoxes;
};

#endif // FINDTYPEAPARTMENT_H
