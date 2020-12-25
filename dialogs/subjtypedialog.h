#ifndef SUBJECTDIALOG_H
#define SUBJECTDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/subjtypemodel.h"
#include <QDialog>
#include <QShortcut>
#include <QSortFilterProxyModel>

class SubjtypeDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit SubjtypeDialog(QWidget *parent = nullptr);
    ~SubjtypeDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

private:
    SubjtypeModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

protected slots:
    virtual int choice(const QItemSelection &selected) const override { return 0; };
};

#endif // SUBJECTDIALOG_H
