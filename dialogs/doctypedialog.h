#ifndef DOCTYPEDIALOG_H
#define DOCTYPEDIALOG_H

#include "models/doctypemodel.h"

#include <QDialog>
#include <QShortcut>
#include <QSortFilterProxyModel>

namespace Ui {
class DoctypeDialog;
}

class DoctypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DoctypeDialog(QWidget *parent = nullptr);
    ~DoctypeDialog();

public slots:
    void edit();
    void insert() ;
    void refresh();
    void remove();

private:
    Ui::DoctypeDialog *ui;

    DoctypeModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    QShortcut *insertShortcut;
    QShortcut *editShortcut;
    QShortcut *removeShortcut;
    QShortcut *refreshShortcut;

    void setupShortcuts();
    void restoreDialogState();
    void saveDialogState();

private slots:
    void changeCurrent(const QModelIndex &current, const QModelIndex &);
    void contextMenu(const QPoint &pos);

    void accept() override;
    void reject() override;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // DOCTYPEDIALOG_H
