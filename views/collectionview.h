#ifndef RECORDVIEW_H
#define RECORDVIEW_H

#include "models/authoritytreemodel.h"
#include "models/authorityproxymodel.h"
#include "models/recordtreemodel.h"
#include "models/recordproxymodel.h"
#include "views/view.h"

#include <QShortcut>

namespace Ui {
class CollectionView;
}

class CollectionView : public View
{
    Q_OBJECT

public:
    explicit CollectionView(QWidget *parent = nullptr);
    ~CollectionView();

    enum {CollectionAuthority, CollectionRecord, CollectionYear, CollectionMember};

    void restoreViewState() override;
    void saveViewState() override;

    void initialize() override;

private:
    Ui::CollectionView *ui;

    AuthorityTreeModel *m_authorityModel = nullptr;
    AuthorityProxyModel *m_authorityProxyModel = nullptr;

    RecordTreeModel *m_recordModel = nullptr;
    RecordProxyModel *m_recordProxyModel = nullptr;

    QShortcut *m_refreshShortcut;

    void setCollectionAuthority();
    void setCollectionRecord();

private slots:
    void contextMenu(const QPoint &pos);
    void switchModel(int index);

};

#endif // RECORDVIEW_H
