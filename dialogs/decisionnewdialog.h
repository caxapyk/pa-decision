#ifndef DECISIONNEWDIALOG_H
#define DECISIONNEWDIALOG_H

#include "dialogs/decisionbasedialog.h"

class DecisionNewDialog : public DecisionBaseDialog
{
    Q_OBJECT

public:
    explicit DecisionNewDialog(QWidget *parent = nullptr);
    ~DecisionNewDialog();

    void setAuthorityId(int id);

private slots:
    void insert();
};

#endif // DECISIONNEWDIALOG_H
