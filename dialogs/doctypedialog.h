#ifndef DOCTYPEDIALOG_H
#define DOCTYPEDIALOG_H

#include "dialogs/referencedialoginterface.h"
#include "helpers/standartviewactions.h"

#include <QDialog>
#include <QSqlTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class DoctypeDialog;
}

class DoctypeDialog : public ReferenceDialogInterface
{
    Q_OBJECT

public:
    explicit DoctypeDialog(QWidget *parent = nullptr);
    ~DoctypeDialog();

public slots:
    void edit() override;
    void insert() override;
    void refresh() override;
    void remove() override;

private:
    Ui::DoctypeDialog *ui;

    QSqlTableModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    StandartViewActions *actions;

private slots:
    void changeCurrent(const QModelIndex &current, const QModelIndex &);
    void contextMenu(const QPoint &pos);

};

#endif // DOCTYPEDIALOG_H
