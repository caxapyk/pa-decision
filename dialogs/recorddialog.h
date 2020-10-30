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
    RecordProxyModel *m_proxyModel;

    QShortcut *insertShortcut;
    QShortcut *editShortcut;
    QShortcut *removeShortcut;
    QShortcut *refreshShortcut;

    void setInfoText();
    void setupShortcuts();
    void restoreDialogState();
    void saveDialogState();

private slots:
    void changeCurrent(const QModelIndex &current, const QModelIndex &);
    void contextMenu(const QPoint &pos);
    void edit();
    void editComment();
    void editFundName();
    void insert();
    void refresh();
    void remove();

    void accept() override;
    void reject() override;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // RECORDDIALOG_H
