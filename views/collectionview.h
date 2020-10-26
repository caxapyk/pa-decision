#ifndef RECORDVIEW_H
#define RECORDVIEW_H

#include "models/recordtreemodel.h"
#include "models/recordproxymodel.h"
#include "views/view.h"

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

private:
    Ui::CollectionView *ui;

    RecordTreeModel *m_recordModel = nullptr;
    RecordProxyModel *m_recordProxyModel = nullptr;

private slots:
    void switchModel();
};

#endif // RECORDVIEW_H
