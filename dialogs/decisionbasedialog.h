#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

#include "models/decisionmodel.h"

#include "models/authorityflatmodel.h"
#include "models/doctypemodel.h"

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
    DoctypeModel *m_doctypeModel;
    DecisionModel *m_decisionModel;

    int recordId = 0;

    void initialize();
    void chooseRecord();
};

#endif // DECISIONBASEDIALOG_H
