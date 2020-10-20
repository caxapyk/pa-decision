#ifndef AUTHORITYVIEW_H
#define AUTHORITYVIEW_H

#include "models/authoritymodel.h"
#include "views/view.h"

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

    AuthorityModel *m_authorityModel;

    void initialize() override;
};

#endif // AUTHORITYVIEW_H
