#ifndef INVENTORYDETAILSDIALOG_H
#define INVENTORYDETAILSDIALOG_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QVariant>
#include <QSqlTableModel>

namespace Ui {
class InventoryDetailsDialog;
}

class InventoryDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InventoryDetailsDialog(const QVariant &id, QWidget *parent = nullptr);
    ~InventoryDetailsDialog();

    int exec() override;

    QString getNumber() const;
    QString getName() const;

public slots:
    void reject() override;

private:
    Ui::InventoryDetailsDialog *ui;
    QDataWidgetMapper *m_mapper = nullptr;

    QVariant m_id;
    QSqlTableModel m_model;

    bool validate();
};

#endif // INVENTORYDETAILSDIALOG_H
