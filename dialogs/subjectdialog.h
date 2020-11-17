#ifndef SUBJECTDIALOG_H
#define SUBJECTDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/subjectmodel.h"

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
    SubjectModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

protected slots:
    virtual int choice(const QItemSelection &selected) const override { return 0; };
};

#endif // SUBJECTDIALOG_H
