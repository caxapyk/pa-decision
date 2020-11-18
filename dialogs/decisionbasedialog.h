#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

#include "models/decisionmodel.h"
#include "models/authorityflatmodel.h"
#include "models/documenttypemodel.h"

#include <QComboBox>
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

    void setChosenId(QComboBox *cb, int id, int column = 0);

protected:
    Ui::DecisionBaseDialog *ui;

private:
    AuthorityFlatModel *m_authorityModel;
    DocumentTypeModel *m_doctypeModel;
    DecisionModel *m_decisionModel;

    int recordId = 0;

    void initialize();
    void chooseRecord();
};

#endif // DECISIONBASEDIALOG_H
