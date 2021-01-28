#ifndef SUBJECTTYPEDIALOG_H
#define SUBJECTTYPEDIALOG_H

#include "dialogs/choicedialog.h"
#include "views/subjecttypeview.h"

class SubjectTypeDialog : public ChoiceDialog
{
    Q_OBJECT

public:
    explicit SubjectTypeDialog(QWidget *parent = nullptr);
    ~SubjectTypeDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

private:
    SubjectTypeView *m_view;
};

#endif // SUBJECTTYPEDIALOG_H
