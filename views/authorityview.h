#ifndef AUTHORITYVIEW_H
#define AUTHORITYVIEW_H

#include "models/authoritytreemodel.h"
#include "models/authorityproxymodel.h"
#include "models/recordtreemodel.h"
#include "models/recordproxymodel.h"
#include "views/view.h"

#include <QAction>
#include <QIcon>
#include <QShortcut>

namespace Ui {
class AuthorityView;
}

class AuthorityView : public View
{
    Q_OBJECT

public:
    explicit AuthorityView(QWidget *parent = nullptr);
    ~AuthorityView();

    void restoreViewState() override;
    void saveViewState() override;

private:
    Ui::AuthorityView *ui;

    AuthorityTreeModel *m_authorityModel;
    AuthorityProxyModel *m_authorityProxyModel;

    QShortcut *openShortcut;
    QShortcut *refreshShortcut;
    QShortcut *insertShortcut;
    QShortcut *editShortcut;
    QShortcut *removeShortcut;

    QModelIndex rootItem;

    void initialize() override;

    void contextMenu();
    void setupShortcuts();

private slots:
    void changeCurrent(const QModelIndex &current, const QModelIndex &previous);
    void open();
    void refresh();
    void insert();
    void edit();
    void remove();
    void info();
};

#endif // AUTHORITYVIEW_H
