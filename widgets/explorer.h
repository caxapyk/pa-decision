#ifndef EXPLORER_H
#define EXPLORER_H

#include "widgets/tab.h"

#include <QTabWidget>

class Explorer : public QTabWidget
{
    Q_OBJECT

public:
    Explorer(QWidget *parent = nullptr);

    void createTab(Tab *tab, const QString &title, QIcon icon = QIcon());
    void closeTab(int index);
    Tab *currentTab();
};

#endif // EXPLORER_H
