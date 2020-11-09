#ifndef PROTOCOLDETAILSDIALOG_H
#define PROTOCOLDETAILSDIALOG_H

#include <QDialog>

namespace Ui {
class ProtocolDetailsDialog;
}

class ProtocolDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProtocolDetailsDialog(QVariant id, QWidget *parent = nullptr);
    ~ProtocolDetailsDialog();

private:
    Ui::ProtocolDetailsDialog *ui;
};

#endif // PROTOCOLDETAILSDIALOG_H
