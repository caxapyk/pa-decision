#ifndef AUTHORITYDIALOG_H
#define AUTHORITYDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/authoritymodel.h"
#include "models/authorityproxymodel.h"

#include <QDialog>
#include <QPushButton>


class AuthorityDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit AuthorityDialog(QWidget *parent = nullptr);
    ~AuthorityDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

public slots:
    void edit() override;
    void insert() override;
    void refresh() override;
    void remove() override;

private:
    AuthorityModel *m_model;
    AuthorityProxyModel *m_proxyModel;

    QPushButton *pB_details;

private slots:
    void changeCurrent(const QModelIndex &current, const QModelIndex &) override;
    void details();
};

#endif // AUTHORITYDIALOG_H
