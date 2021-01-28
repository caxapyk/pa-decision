#ifndef DOCUMENTTYPEDIALOG_H
#define DOCUMENTTYPEDIALOG_H

#include "dialogs/choicedialog.h"
#include "views/documenttypeview.h"

class DocumentTypeDialog : public ChoiceDialog
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
