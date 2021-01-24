#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

#include "dialogs/treedialog.h"
#include "models/documenttypemodel.h"

#include <QDialog>
#include <QComboBox>

namespace Ui {
class DecisionFormDialog;
}

class DecisionFormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DecisionFormDialog(const QVariant &authorityId, const QVariant &id = QVariant(), QWidget *parent = nullptr);
    ~DecisionFormDialog();

    int exec() override;

    void restoreDialogState();
    void saveDialogState();

    //bool setChosenId(QComboBox *cb, int id, int column = 0);

    bool validate();

    QVariant id() const { return m_id; };

public slots:
    void accept() override;
    void reject() override;

protected:
    Ui::DecisionFormDialog *ui;

private:
    QVariant m_authorityId;
    QVariant m_id;

    QList<QVariant> m_authorityIds;
    QList<QVariant> m_doctypeIds;
    QList<QVariant> m_fundIds;
    QList<QVariant> m_inventoryIds;
    QList<QVariant> m_recordIds;
    QList<QVariant> m_protocolIds;

    void initialize();
    void updateAuthority();
    void updateDoctype();
    void updateFund();

private slots:
    void updateInventory(int fundIndex);
    void updateRecord(int recordIndex);
    void updateProtocol(int inventoryIndex);

    void chooseFund();
    void chooseRecord();
    void chooseInventory();
    void chooseProtocol();

    void useProtocolStateChanged(bool checked);

    void save();
};

#endif // DECISIONBASEDIALOG_H
