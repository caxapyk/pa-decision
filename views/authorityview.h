#ifndef NAVIGATORVIEW_H
#define NAVIGATORVIEW_H

#include "models/authoritymodel.h"
#include "models/authorityproxymodel.h"
#include "views/view.h"
#include "views/explorerview.h"
#include "views/decisionview.h"
#include "utils/basecontextmenu.h"
#include "widgets/authoritytree.h"

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
    ExplorerView *_explorer;
    AuthorityTree *m_tree;

    AuthorityModel *m_authorityModel;
    AuthorityProxyModel *m_authorityProxyModel;

    QVariant m_authorityId;

private slots:
    void selected(const QItemSelection &selected, const QItemSelection &deselected);
    void openInNewTab(const QModelIndex &index);
    void insert();
    void edit();
    void remove();
    void refresh();
    void details();
};

#endif // NAVIGATORVIEW_H
