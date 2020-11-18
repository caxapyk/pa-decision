#ifndef NAVIGATORVIEW_H
#define NAVIGATORVIEW_H

#include "models/authoritymodel.h"
#include "models/authorityproxymodel.h"
#include "models/referencemodel.h"
#include "views/view.h"
#include "views/explorerview.h"

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
    enum Group {GroupRecord, GroupProtocol, GroupDoctype, GroupYear, GroupSubject};

    explicit NavigatorView(QWidget *parent = nullptr);
    ~NavigatorView();

    void initialize() override;

    void restoreViewState() override;
    void saveViewState() override;

    int currentGroup() { return currentGroupIndex; };

    void openIndexTab();

    void refreshAuthority();
    void refreshGroup();

    ExplorerView *explorer() { return _explorer; };
    void setExplorer(ExplorerView *exp);

private:
    Ui::NavigatorView *ui;
    QShortcut *m_refreshShortcut;

    AuthorityModel *m_authorityModel;
    AuthorityProxyModel *m_authorityProxyModel;

    ReferenceModel *m_collectionModel = nullptr;
    QSortFilterProxyModel *m_collectionProxyModel = nullptr;

    int currentGroupIndex = 0;

    ExplorerView *_explorer;

private slots:
    void contextMenu(const QPoint &pos);
    void loadGroup(int collection);
    void openInNewTab();
};

#endif // NAVIGATORVIEW_H
