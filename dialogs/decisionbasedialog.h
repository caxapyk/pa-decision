#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

#include "dialogs/detailsdialog.h"
#include "dialogs/referencedialog.h"
#include "models/authorityflatmodel.h"
#include "models/documenttypemodel.h"
#include "models/protocolflatmodel.h"
#include "models/recordflatmodel.h"

#include <QComboBox>
#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class DecisionBaseDialog;
}

class DecisionBaseDialog : public DetailsDialog
{
    Q_OBJECT

public:
    explicit DecisionBaseDialog(QWidget *parent = nullptr);
    ~DecisionBaseDialog();

    void restoreDialogState();
    void saveDialogState();

    bool setChosenId(QComboBox *cb, int id, int column = 0);

    AuthorityFlatModel *authorityModel() { return m_authorityModel; };
    DocumentTypeModel *documentTypeModel() { return m_doctypeModel; };
    ProtocolFlatModel *protocolModel() { return m_protocolModel; };
    RecordFlatModel *recordModel() { return m_recordModel; };

    bool validate();

public slots:
    void reject() override;

protected:
    Ui::DecisionBaseDialog *ui;

private:
    AuthorityFlatModel *m_authorityModel;
    DocumentTypeModel *m_doctypeModel;
    ProtocolFlatModel *m_protocolModel;
    RecordFlatModel *m_recordModel;

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
