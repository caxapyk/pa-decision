#ifndef NAVIGATORVIEW_H
#define NAVIGATORVIEW_H

#include "models/authoritymodel.h"
#include "models/authorityproxymodel.h"
#include "models/referencemodel.h"
#include "views/view.h"
#include "views/explorerview.h"
#include "views/decisionview.h"

#include <QShortcut>
#include <QSortFilterProxyModel>

namespace Ui {
class NavigatorView;
}

class NavigatorView : public View
{
    Q_OBJECT

public:
    enum Tabs {TabGroup, TabSearch};

    explicit NavigatorView(QWidget *parent = nullptr);
    ~NavigatorView();

    void initialize() override;

    void restoreViewState() override;
    void saveViewState() override;

    void openIndexTab();
    DecisionView *currentDecisionView();

    ExplorerView *explorer() { return _explorer; };
    void setExplorer(ExplorerView *exp) { _explorer = exp; };

private:
    Ui::NavigatorView *ui;
    QShortcut *m_refreshShortcut;

    AuthorityModel *m_authorityModel;
    AuthorityProxyModel *m_authorityProxyModel;

    ExplorerView *_explorer;

private slots:
    void contextMenu(const QPoint &pos);
    void openInNewTab(const QModelIndex &index);
    void refresh();
};

#endif // NAVIGATORVIEW_H
