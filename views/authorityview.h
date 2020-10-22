#ifndef AUTHORITYVIEW_H
#define AUTHORITYVIEW_H

#include "models/authoritytreemodel.h"
#include "views/view.h"

#include <QMenu>
#include <QWidget>
#include <QAction>
#include <QIcon>

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

    QAction openAction;
    QAction refreshAction;
    QAction appendAction;
    QAction editAction;
    QAction removeAction;
    QAction infoAction;

    void initialize() override;

    void contextMenu();
    QModelIndex currentIndex();
    void setupActions();

private slots:
    void open(const QModelIndex &index);
    void refresh();
    void insert();
    void edit(const QModelIndex &index);
    void remove(const QModelIndex &index);
    void info();
};

#endif // AUTHORITYVIEW_H
