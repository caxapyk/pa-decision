#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include "models/recordtreemodel.h"
#include "models/recordproxymodel.h"

#include <QDialog>
#include <QShortcut>

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

    QShortcut *m_refreshShortcut;

private slots:
    void contextMenu(const QPoint &pos);
    void edit();
    void insert();
    void refresh();
    void remove();
    void title();
};

#endif // RECORDDIALOG_H
