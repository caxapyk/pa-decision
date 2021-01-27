#ifndef SUBJECTDIALOG_H
#define SUBJECTDIALOG_H

#include "dialogs/choicedialog.h"
#include "views/subjecttypetreeview.h"

class SubjtypeDialog : public ChoiceDialog
{
    Q_OBJECT

public:
    explicit SubjtypeDialog(QWidget *parent = nullptr);
    ~SubjtypeDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

private:
    SubjectTypeTreeView *m_view;
};

#endif // SUBJECTDIALOG_H
