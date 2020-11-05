#ifndef NAVIGATORVIEW_H
#define NAVIGATORVIEW_H

#include "models/collectionmodel.h"
#include "views/view.h"

#include <QShortcut>
#include <QSortFilterProxyModel>

namespace Ui {
class NavigatorView;
}

class NavigatorView : public View
{
    Q_OBJECT

public:
    enum Tabs {TabCollection, TabSearch};
    enum Collection {CollectionAuthority, CollectionRecord, CollectionProtocol, CollectionYear, CollectionObject};

    explicit NavigatorView(QWidget *parent = nullptr);
    ~NavigatorView();

    void initialize() override;

    void restoreViewState() override;
    void saveViewState() override;

    int currentCollection() {return current;};
    void refresh();

private:
    Ui::NavigatorView *ui;
    QShortcut *m_refreshShortcut;

    CollectionModel *m_model = nullptr;
    QSortFilterProxyModel *m_proxyModel = nullptr;

    int current = 0;

private slots:
    void contextMenu(const QPoint &pos);
    void switchModel(int index);
};

#endif // NAVIGATORVIEW_H
