#ifndef PROTOCOLDETAILSDIALOG_H
#define PROTOCOLDETAILSDIALOG_H

#include "models/protocolmodel.h"

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>


namespace Ui {
class ProtocolDetailsDialog;
}

class ProtocolDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProtocolDetailsDialog(QVariant id = QVariant(), QWidget *parent = nullptr);
    ~ProtocolDetailsDialog();

    void setAuthority(int id) { aid = QVariant(id); };

public slots:
    void reject() override;

private:
    Ui::ProtocolDetailsDialog *ui;

    QSqlTableModel *m_model;
    QDataWidgetMapper *m_mapper = nullptr;

    bool _form_changed = false;
    QVariant aid;

private slots:
    void insert();
};

#endif // PROTOCOLDETAILSDIALOG_H
