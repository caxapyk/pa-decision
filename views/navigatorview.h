#ifndef NAVIGATORVIEW_H
#define NAVIGATORVIEW_H

#include "views/collectionview.h"
#include "views/view.h"

namespace Ui {
class NavigatorView;
}

class NavigatorView : public View
{
    Q_OBJECT

public:
    enum {TabCollection, TabSearch};

    explicit NavigatorView(QWidget *parent = nullptr);
    ~NavigatorView();

    void restoreViewState() override;
    void saveViewState() override;

    void tabSwitch(int index);

private:
    Ui::NavigatorView *ui;

    CollectionView *m_collectionView = nullptr;
};

#endif // NAVIGATORVIEW_H
