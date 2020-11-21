#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

#include "dialogs/detailsdialog.h"
#include "dialogs/referencedialog.h"
#include "models/authorityflatmodel.h"
#include "models/documenttypemodel.h"
#include "models/protocolflatmodel.h"
#include "models/recordflatmodel.h"

#include <QDataWidgetMapper>
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

    int exec() override;

    void restoreDialogState();
    void saveDialogState();

    bool setChosenId(QComboBox *cb, int id, int column = 0);

    bool validate();

    QVariant id() const { return m_id; };

public slots:
    void accept() override;
    void reject() override;

protected:
    Ui::DecisionBaseDialog *ui;

private:
    AuthorityFlatModel *m_authorityModel = nullptr;
    DocumentTypeModel *m_doctypeModel = nullptr;
    ProtocolFlatModel *m_protocolModel = nullptr;
    RecordFlatModel *m_recordModel = nullptr;

    QDataWidgetMapper *m_mapper = nullptr;

    QVariant m_id;

private slots:
    void accessChanged(int index);
    //void authorityChanged(int index);
    void protocolStateChanged(bool on);

    void openDoctypeDialog();
    void openProtocolDialog();
    void openRecordDialog();

    void openExternalDialog(QComboBox *cb, ReferenceDialog *dialog);

    void save();
};

#endif // DECISIONBASEDIALOG_H
