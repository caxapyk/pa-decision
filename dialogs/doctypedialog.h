#ifndef DOCTYPEDIALOG_H
#define DOCTYPEDIALOG_H

#include "dialogs/treedialog.h"
#include "models/documenttypemodel.h"

#include <QDialog>
#include <QShortcut>
#include <QSortFilterProxyModel>

class DoctypeDialog : public TreeDialog
{
    Q_OBJECT

public:
    explicit DoctypeDialog(QWidget *parent = nullptr);
    ~DoctypeDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

private:
    DocumentTypeModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    bool choiceButtonEnabled() override;
};

#endif // DOCTYPEDIALOG_H
