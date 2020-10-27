#ifndef RECORDVIEW_H
#define RECORDVIEW_H

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

    enum {CollectionRecord, CollectionYear, CollectionMember};

    void restoreViewState() override;
    void saveViewState() override;

    void initialize() override;

private:
    Ui::CollectionView *ui;

    RecordTreeModel *m_recordModel = nullptr;
    RecordProxyModel *m_recordProxyModel = nullptr;

    QShortcut *m_refreshShortcut;

    void setRecordCollection();

private slots:
    void contextMenu(const QPoint &pos);
    void switchModel(int index);
};

#endif // RECORDVIEW_H
