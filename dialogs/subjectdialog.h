#ifndef SUBJECTDIALOG_H
#define SUBJECTDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/documenttypemodel.h"

#include <QDialog>
#include <QShortcut>
#include <QSortFilterProxyModel>

class SubjectDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit SubjectDialog(QWidget *parent = nullptr);
    ~SubjectDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

private:
    DocumentTypeModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

protected slots:
    virtual void selected(const QItemSelection &selected, const QItemSelection &deselected) override;
    virtual int choice(const QItemSelection &selected) const override { return 0; };
};

#endif // SUBJECTDIALOG_H
