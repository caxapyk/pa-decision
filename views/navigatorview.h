#ifndef NAVIGATORVIEW_H
#define NAVIGATORVIEW_H

#include "views/authorityview.h"
#include "views/collectionview.h"
#include "views/view.h"

namespace Ui {
class NavigatorView;
}

class NavigatorView : public View
{
    Q_OBJECT

public:
    explicit NavigatorView(QWidget *parent = nullptr);
    ~NavigatorView();

    void restoreViewState() override;
    void saveViewState() override;

    enum {TabAuthority, TabCollection, TabSearch};

private:
    Ui::NavigatorView *ui;

    AuthorityView *m_authorityView = nullptr;
    CollectionView *m_collectionView = nullptr;

    void tabSwitch(int index);

};

#endif // NAVIGATORVIEW_H
