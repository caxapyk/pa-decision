#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include "models/recordtreemodel.h"
#include "models/recordproxymodel.h"

#include <QDialog>

namespace Ui {
class RecordDialog;
}

class RecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecordDialog(QWidget *parent = nullptr);
    ~RecordDialog();

private:
    Ui::RecordDialog *ui;

    RecordTreeModel *m_model;
    RecordProxyModel *m_proxy;
};

#endif // RECORDDIALOG_H
