#ifndef DOCUMENTTYPEDIALOG_H
#define DOCUMENTTYPEDIALOG_H

#include "dialogs/referencedialog.h"
#include "views/documenttypeview.h"

class DocumentTypeDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit DocumentTypeDialog(QWidget *parent = nullptr);
    ~DocumentTypeDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

private:
    DocumentTypeView *m_view;
};

#endif // DOCUMENTTYPEDIALOG_H
