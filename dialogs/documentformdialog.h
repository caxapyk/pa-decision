#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

#include "dialogs/choicedialog.h"
#include "models/documenttypemodel.h"
#include "widgets/subjecttable.h"

#include <QDialog>
#include <QComboBox>

namespace Ui {
class DocumentFormDialog;
}

class DocumentFormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocumentFormDialog(const QVariant &authorityId, const QVariant &id = QVariant(), QWidget *parent = nullptr);
    ~DocumentFormDialog();

    void restoreDialogState();
    void saveDialogState();

    QVariant getId() const { return m_id; };
    QVariant getAuthority() const;
    QVariant getDoctype() const;
    QVariant getNumber() const;
    QVariant getDate() const;
    QVariant getTitle() const;
    QVariant getPages() const;
    QVariant getAnnexes() const;
    QVariant getFund() const;
    QVariant getInventory() const;
    QVariant getRecord() const;
    QVariant getProtocol() const;
    QVariant getProtocolPage() const;
    QVariant getContent() const;
    QVariant getComment() const;
    QVariant getAccess() const;

    bool validate();
    void save();

public slots:
    void accept() override;
    void reject() override;

private:
    Ui::DocumentFormDialog *ui;
    SubjectTable *m_subjectsTable;

    QVariant m_authorityId;
    QVariant m_id;

    QVector<QVariant> m_authorityIds;
    QVector<QVariant> m_doctypeIds;
    QVector<QVariant> m_fundIds;
    QVector<QVariant> m_inventoryIds;
    QVector<QVariant> m_recordIds;
    QVector<QVariant> m_protocolIds;

    void initialize();
    void setValues();
    void updateAuthority();
    void updateDoctype();
    void updateFund();

    bool saveForm();
    bool saveSubjects();

private slots:
    void updateInventory(int fundIndex);
    void updateRecord(int recordIndex);
    void updateProtocol(int inventoryIndex);

    void chooseFund();
    void chooseRecord();
    void chooseInventory();
    void chooseProtocol();

    void useProtocolStateChanged(bool checked);
    void accessStateChanged(int index);
};

#endif // DECISIONBASEDIALOG_H
