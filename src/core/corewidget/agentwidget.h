#ifndef AGENTWIDGET_H
#define AGENTWIDGET_H

#include <QWidget>
#include "mainwidget.h"

namespace Ui {
class AgentWidget;
}

class AgentWidget : public MainWidget
{
    Q_OBJECT
    
public:
    explicit AgentWidget(QWidget *parent = 0);
    ~AgentWidget();
    void load(int aIdObjects);
public slots:
    void save();
private:
    Ui::AgentWidget *ui;
    int mId;
    QList<int> mRolesIds;
};

#endif // AGENTWIDGET_H
