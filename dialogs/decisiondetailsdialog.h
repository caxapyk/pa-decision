#ifndef DECISIONBASEDIALOG_H
#define DECISIONBASEDIALOG_H

#include "dialogs/treedialog.h"
#include "models/authorityflatmodel.h"
#include "models/documenttypemodel.h"
#include "models/protocolflatmodel.h"
#include "models/recordflatmodel.h"

#include <QDialog>
#include <QDataWidgetMapper>
#include <QComboBox>
#include <QSqlRecord>
#include <QSqlRelationalTableModel>

namespace Ui {
class DecisionDetailsDialog;
}

class DecisionDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DecisionDetailsDialog(QVariant id = QVariant(), QWidget *parent = nullptr);
    ~DecisionDetailsDialog();

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
    Ui::DecisionDetailsDialog *ui;

private:
    QSqlRelationalTableModel *m_model;
    AuthorityFlatModel *m_authorityModel;
    DocumentTypeModel *m_doctypeModel;
    ProtocolFlatModel *m_protocolModel;
    RecordFlatModel *m_recordModel;

    QDataWidgetMapper *m_mapper = nullptr;

    QVariant m_id;

    void initialize();

private slots:
    void authorityChanged(int index);
    void accessChanged(int index);
    void protocolStateChanged(bool on);

    void openDoctypeDialog();
    void openProtocolDialog();
    void openRecordDialog();

    void openExternalDialog(QComboBox *cb, TreeDialog *dialog);

    void save();
};

#endif // DECISIONBASEDIALOG_H
