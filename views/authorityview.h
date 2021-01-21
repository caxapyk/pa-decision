#ifndef NAVIGATORVIEW_H
#define NAVIGATORVIEW_H

#include "models/authoritymodel.h"
#include "models/authorityproxymodel.h"
#include "models/sqlbasemodel.h"
#include "views/view.h"
#include "views/explorerview.h"
#include "views/decisionview.h"

#include <QShortcut>
#include <QSortFilterProxyModel>

namespace Ui {
class AuthorityView;
}

class AuthorityView : public View
{
    Q_OBJECT

public:
    enum Tabs {TabGroup, TabSearch};

    explicit AuthorityView(QWidget *parent = nullptr);
    ~AuthorityView();

    void initialize() override;

    void restoreViewState() override;
    void saveViewState() override;

    void openIndexTab();
    DecisionView *currentDecisionView();

    QVariant id() const { return m_authorityId; };

    ExplorerView *explorer() { return _explorer; };
    void setExplorer(ExplorerView *exp) { _explorer = exp; };

private:
    Ui::AuthorityView *ui;
    QShortcut *insertShortcut = nullptr;
    QShortcut *editShortcut = nullptr;
    QShortcut *removeShortcut = nullptr;
    QShortcut *refreshShortcut = nullptr;

    AuthorityModel *m_authorityModel;
    AuthorityProxyModel *m_authorityProxyModel;

    ExplorerView *_explorer;

    QVariant m_authorityId;

    void setupShortcuts();

private slots:
    void contextMenu(const QPoint &pos);
    void selected(const QItemSelection &selected, const QItemSelection &deselected);
    void openInNewTab(const QModelIndex &index);
    void insert();
    void edit();
    void remove();
    void refresh();
    void details();
};

#endif // NAVIGATORVIEW_H
