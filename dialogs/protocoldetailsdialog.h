#ifndef PROTOCOLDETAILSDIALOG_H
#define PROTOCOLDETAILSDIALOG_H

#include "models/protocolmodel.h"

#include <QDataWidgetMapper>
#include <QDialog>

namespace Ui {
class ProtocolDetailsDialog;
}

class ProtocolDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProtocolDetailsDialog(ProtocolModel *model, int row = -1,  QWidget *parent = nullptr);
    ~ProtocolDetailsDialog();

    void accept() override;
    void reject() override;

    QMap<QString, QVariant> property() { return prop; };

private:
    Ui::ProtocolDetailsDialog *ui;

    QDataWidgetMapper *m_mapper = nullptr;
    ProtocolModel *m_model;

    QMap<QString, QVariant> prop;
    int mrow;
};

#endif // PROTOCOLDETAILSDIALOG_H
