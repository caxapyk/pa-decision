#ifndef AUTHORITYDIALOG_H
#define AUTHORITYDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/_authoritymodel.h"

#include <QDialog>
#include <QPushButton>
#include <QSortFilterProxyModel>


class _AuthorityDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit _AuthorityDialog(QWidget *parent = nullptr);
    ~_AuthorityDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

private:
    _AuthorityModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    QPushButton *pB_details;

protected slots:
    virtual void selected(const QModelIndex &current, const QModelIndex &) override;
    virtual int choice(const QModelIndex &current) const override { return 0; };

private slots:
    void details();
};

#endif // AUTHORITYDIALOG_H
