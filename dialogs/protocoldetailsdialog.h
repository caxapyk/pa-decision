#ifndef PROTOCOLDETAILSDIALOG_H
#define PROTOCOLDETAILSDIALOG_H

#include "dialogs/detailsdialog.h"
#include "models/protocolmodel.h"

#include <QDialog>
#include <QDataWidgetMapper>
#include <QSqlRecord>

namespace Ui {
class ProtocolDetailsDialog;
}

class ProtocolDetailsDialog : public DetailsDialog
{
    Q_OBJECT

public:
    explicit ProtocolDetailsDialog(QWidget *parent = nullptr);
    ~ProtocolDetailsDialog();

    int exec() override;

public slots:
    void reject() override;

private:
    Ui::ProtocolDetailsDialog *ui;
    QDataWidgetMapper *m_mapper = nullptr;

    bool _form_changed = false;

private slots:
    void insert();
    void setRecord(int row, QSqlRecord &record);
};

#endif // PROTOCOLDETAILSDIALOG_H
