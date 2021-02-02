#ifndef DOCUMENTFORMDIALOG_H
#define DOCUMENTFORMDIALOG_H

#include "dialogs/choicedialog.h"
#include "models/documenttypemodel.h"
#include "views/subjectview.h"

#include <QDialog>
#include <QComboBox>

namespace Ui {
class DocumentFormDialog;
}

class DocumentFormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocumentFormDialog(QWidget *parent = nullptr);
    ~DocumentFormDialog();

    void restoreDialogState();
    void saveDialogState();

    QVariant authorityId() const { return m_authorityId; };
    void setAuthorityId(const QVariant &id) { m_authorityId = id; };

    QVariant id() const { return m_id; };
    void setId(const QVariant &id) { m_id = id; };

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

    int exec() override;

public slots:
    void accept() override;
    void reject() override;

private:
    Ui::DocumentFormDialog *ui;
    SubjectView *m_subjectView;

    QVariant m_authorityId;
    QVariant m_id;

    bool m_stateChanged = false;

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

    void stateChanged();
};

#endif // DOCUMENTFORMDIALOG_H
