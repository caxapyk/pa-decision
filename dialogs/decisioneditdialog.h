#ifndef DecisionEditDialog_H
#define DecisionEditDialog_H

#include "dialogs/decisionbasedialog.h"

#include <QDataWidgetMapper>

class DecisionEditDialog : public DecisionBaseDialog
{
    Q_OBJECT

public:
    explicit DecisionEditDialog(QWidget *parent = nullptr);
    ~DecisionEditDialog();

    int exec() override;

private:
    QDataWidgetMapper *m_mapper;
};

#endif // DecisionEditDialog_H
