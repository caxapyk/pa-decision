#ifndef DOCTYPEDIALOG_H
#define DOCTYPEDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/documenttypemodel.h"

#include <QDialog>
#include <QShortcut>
#include <QSortFilterProxyModel>

class DoctypeDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit DoctypeDialog(QWidget *parent = nullptr);
    ~DoctypeDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

private:
    StandardReferenceModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

protected slots:
    virtual void selected(const QModelIndex &current, const QModelIndex &) override;
    virtual int choice(const QModelIndex &current) const override { return 0; };
};

#endif // DOCTYPEDIALOG_H
