#ifndef AUTHORITYVIEW_H
#define AUTHORITYVIEW_H

#include "models/authoritytreemodel.h"
#include "views/view.h"

#include <QMenu>
#include <QWidget>

namespace Ui {
class AuthorityView;
}

class AuthorityView : public View
{
    Q_OBJECT

public:
    explicit AuthorityView(QWidget *parent = nullptr);
    ~AuthorityView();

private:
    Ui::AuthorityView *ui;

    AuthorityTreeModel *m_authorityModel;

    void initialize() override;

    void showContextMenu();
};

#endif // AUTHORITYVIEW_H
