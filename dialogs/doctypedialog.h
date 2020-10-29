#ifndef DOCTYPEDIALOG_H
#define DOCTYPEDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
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

    QSqlTableModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    QShortcut *insertShortcut;
    QShortcut *editShortcut;
    QShortcut *removeShortcut;
    QShortcut *refreshShortcut;

    void setupShortcuts();

private slots:
    void changeCurrent(const QModelIndex &current, const QModelIndex &);
    void contextMenu(const QPoint &pos);

};

#endif // DOCTYPEDIALOG_H
