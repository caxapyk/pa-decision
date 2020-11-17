#ifndef EXPLORERVIEW_H
#define EXPLORERVIEW_H

#include "views/view.h"
#include "views/decisionview.h"

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

    void createTab(View *view, const QString &title);

private:
    Ui::ExplorerView *ui;
};

#endif // EXPLORERVIEW_H
