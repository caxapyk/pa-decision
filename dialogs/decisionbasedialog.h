#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/decisionmodel.h"
#include "models/authorityflatmodel.h"
#include "models/documenttypemodel.h"
#include "models/protocolmodel.h"

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

    bool setChosenId(QComboBox *cb, int id, int column = 0);

protected:
    Ui::DecisionBaseDialog *ui;

    void openExternal(QComboBox *cb, ReferenceDialog *dialog, int col = 0);

private:
    DecisionModel *m_decisionModel;

    AuthorityFlatModel *m_authorityModel;
    DocumentTypeModel *m_doctypeModel;
    ProtocolModel *m_protocolModel;

    int recordId = 0;

    void initialize();

    QVariant m_authorityId;

private slots:
    void authorityChanged(int index);
    void protocolStateChanged(bool on);
};

#endif // DECISIONBASEDIALOG_H
