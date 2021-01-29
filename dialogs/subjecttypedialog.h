#ifndef SUBJECTTYPEDIALOG_H
#define SUBJECTTYPEDIALOG_H

#include "dialogs/referencedialog.h"
#include "views/subjecttypeview.h"

class SubjectTypeDialog : public ReferenceDialog
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
