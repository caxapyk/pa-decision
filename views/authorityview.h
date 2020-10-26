#ifndef AUTHORITYVIEW_H
#define AUTHORITYVIEW_H

#include "models/authoritytreemodel.h"
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
    enum {TabAuthority, TabCollection, TabSearch};
    enum {CollectionRecord, CollectionYear, CollectionMember};

    Ui::AuthorityView *ui;

    AuthorityTreeModel *m_authorityModel;
    RecordTreeModel *m_recordModel = nullptr;

    RecordProxyModel *m_recordProxyModel = nullptr;

    QShortcut *openShortcut;
    QShortcut *refreshShortcut;
    QShortcut *insertShortcut;
    QShortcut *editShortcut;
    QShortcut *removeShortcut;

    void initialize() override;

    void contextMenu();
    void setupShortcuts();
    void tabSwitch(int index);
    void loadCollection();
    void initializeRecord();

private slots:
    void clicked(const QModelIndex &index);
    void open();
    void refresh();
    void insert();
    void edit();
    void remove();
    void info();
};

#endif // AUTHORITYVIEW_H
