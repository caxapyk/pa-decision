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
    void insert() override;
    void refresh() override;

private:
    AuthorityModel *m_model;
    AuthorityProxyModel *m_proxyModel;

    QPushButton *pB_details;

protected slots:
    virtual void selected(const QItemSelection &selected, const QItemSelection &deselected) override;
    virtual int choice(const QItemSelection &selected) const override { return 0; };

private slots:
    void details();
};

#endif // AUTHORITYDIALOG_H
