#ifndef DOCTYPEDIALOG_H
#define DOCTYPEDIALOG_H

#include "dialogs/choicedialog.h"
#include "views/documenttypetreeview.h"

class DoctypeDialog : public ChoiceDialog
{
    Q_OBJECT

public:
    explicit DoctypeDialog(QWidget *parent = nullptr);
    ~DoctypeDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

private:
    DocumentTypeTreeView *m_view;
};

#endif // DOCTYPEDIALOG_H
