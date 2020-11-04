#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

#include "models/authorityflatmodel.h"

#include <QDialog>

namespace Ui {
class DecisionBaseDialog;
}

class DecisionBaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DecisionBaseDialog(QWidget *parent = nullptr);
    ~DecisionBaseDialog();

private:
    Ui::DecisionBaseDialog *ui;

    AuthorityFlatModel *m_authorityModel;

    void initialize();
};

#endif // DECISIONBASEDIALOG_H
