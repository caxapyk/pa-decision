#ifndef FUNDDETAILSDIALOG_H
#define FUNDDETAILSDIALOG_H

#include "dialogs/referencedialog.h"

namespace Ui {
class FundDetailsDialog;
}

class FundDetailsDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit FundDetailsDialog(QWidget *parent = nullptr);
    ~FundDetailsDialog();

private:
    Ui::FundDetailsDialog *ui;
};

#endif // FUNDDETAILSDIALOG_H
