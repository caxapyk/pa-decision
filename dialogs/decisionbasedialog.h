#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/authorityflatmodel.h"
#include "models/documenttypemodel.h"
#include "models/protocolflatmodel.h"
#include "models/recordflatmodel.h"

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

private:
    //DecisionModel *m_decisionModel;

    AuthorityFlatModel *m_authorityModel;
    DocumentTypeModel *m_doctypeModel;
    ProtocolFlatModel *m_protocolModel;
    RecordFlatModel *m_recordModel;

    int recordId = 0;

    QVariant m_authorityId;

    void initialize();

private slots:
    void accessChanged(int index);
    void authorityChanged(int index);
    void protocolStateChanged(bool on);

    void openDoctypeDialog();
    void openProtocolDialog();
    void openRecordDialog();

    void openExternalDialog(QComboBox *cb, ReferenceDialog *dialog);
};

#endif // DECISIONBASEDIALOG_H
