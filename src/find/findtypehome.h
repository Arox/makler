#ifndef FINDTYPEHOME_H
#define FINDTYPEHOME_H

#include <QWidget>
#include <QList>
#include <QComboBox>
#include <QSignalMapper>

namespace Ui {
class FindTypeHome;
}

class FindTypeHome : public QWidget
{
    Q_OBJECT
    
public:
    explicit FindTypeHome(QWidget *parent = 0);
    ~FindTypeHome();
    int areaSize();
    int year();
    int minRoom();
    int maxRoom();
    int minLevel();
    int maxLevel();
    QList<int> material();

    bool isAreaSize();
    bool isYear();
    bool isMinRoom();
    bool isMaxRoom();
    bool isMinLevel();
    bool isMaxLevel();
    bool isMaterial();

    QString sqlWhere();
    QString joinWhere();
protected:
    QList<int> idsComboBox(QComboBox* apBox);
    void clickItemComboBox(QComboBox* apBox, int aIndex);
protected slots:
    void selectComboBox(int aIndex);
private:
    Ui::FindTypeHome *ui;
    QSignalMapper mMapper;
    QList<QComboBox*> mBoxes;
};

#endif // FINDTYPEHOME_H
