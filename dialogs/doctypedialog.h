#ifndef DOCTYPEDIALOG_H
#define DOCTYPEDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/doctypemodel.h"

#include <QDialog>
#include <QShortcut>
#include <QSortFilterProxyModel>

/*namespace Ui {
class DoctypeDialog;
}*/

class DoctypeDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit DoctypeDialog(QWidget *parent = nullptr);
    ~DoctypeDialog();

    void restoreDialogState() override;
    void saveDialogState() override;
    void setupShortcuts() override;

public slots:
    void edit() override;
    void insert() override;
    void refresh() override;
    void remove() override;

private:
    DoctypeModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    QShortcut *insertShortcut;
    QShortcut *editShortcut;
    QShortcut *removeShortcut;
    QShortcut *refreshShortcut;

private slots:
    void changeCurrent(const QModelIndex &current, const QModelIndex &) override;
    void contextMenu(const QPoint &pos) override;

    void accept() override;
    void reject() override;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // DOCTYPEDIALOG_H
