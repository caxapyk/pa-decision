#ifndef EXPLORERVIEW_H
#define EXPLORERVIEW_H

#include "views/view.h"
#include "views/decisionview.h"
#include "widgets/tab.h"

#include <QTabWidget>
#include <QWidget>

namespace Ui {
class ExplorerView;
}

class ExplorerView : public View
{
    Q_OBJECT

public:
    explicit ExplorerView(QWidget *parent = nullptr);
    ~ExplorerView();

    void createTab(Tab *tab, const QString &title);
    void closeTab(int index);

private:
    Ui::ExplorerView *ui;
};

#endif // EXPLORERVIEW_H
