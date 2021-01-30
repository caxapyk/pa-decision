#include "explorer.h"
#include "application.h"
#include "widgets/documenttab.h"

#include <QDebug>

Explorer::Explorer(QWidget *parent) : QTabWidget(parent)
{
    setTabsClosable(true);

    connect(this, &QTabWidget::tabCloseRequested, this, &Explorer::closeTab);
    connect(this, &QTabWidget::currentChanged, this, &Explorer::tabChanged);
}

void Explorer::createTab(Tab *tab, const QString &title, QIcon icon)
{
    addTab(tab, icon, title);
    setCurrentIndex(count() - 1);
}

void Explorer::closeTab(int index)
{
    delete widget(index);
}

Tab *Explorer::currentTab()
{
    int v = currentIndex();
    Tab *tab = dynamic_cast<Tab*>(widget(v));

    return tab;
}

void Explorer::tabChanged(int index)
{
    Tab *tab = dynamic_cast<Tab*>(widget(index));
    DocumentTab *dt = qobject_cast<DocumentTab*>(tab);

    application->mainWindow()->insertAction()->setEnabled(dt != nullptr ? true : false);
    application->mainWindow()->editAction()->setEnabled(false);
    application->mainWindow()->removeAction()->setEnabled(false);
    application->mainWindow()->refreshAction()->setEnabled(dt != nullptr ? true : false);
    application->mainWindow()->printAction()->setEnabled(dt != nullptr && dt->isDockOpen());
    application->mainWindow()->afAction()->setEnabled(dt != nullptr ? true : false);
    application->mainWindow()->searchPanel()->setEnabled(dt != nullptr ? true : false);
}
