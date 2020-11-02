#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

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
};

#endif // DECISIONBASEDIALOG_H
