#include "explorer.h"
#include "application.h"

Explorer::Explorer(QWidget *parent) : QTabWidget(parent)
{
    setTabsClosable(true);

    connect(this, &QTabWidget::tabCloseRequested, this, &Explorer::closeTab);
}

void Explorer::createTab(Tab *tab, const QString &title, QIcon icon)
{
    addTab(tab, icon, title);
    setCurrentIndex(count() - 1);

    application->mainWindow()->action_new->setEnabled(true);
    application->mainWindow()->action_refresh->setEnabled(true);
}

void Explorer::closeTab(int index)
{
    delete widget(index);

    if(count() == 0) {
        application->mainWindow()->action_new->setDisabled(true);
        application->mainWindow()->action_refresh->setDisabled(true);
    }
}

Tab *Explorer::currentTab()
{
    int v = currentIndex();
    Tab *tab = dynamic_cast<Tab*>(widget(v));

    return tab;
}
