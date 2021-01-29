#ifndef RECORDDETAILSDIALOG_H
#define RECORDDETAILSDIALOG_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QVariant>
#include <QSqlTableModel>

namespace Ui {
class RecordDetailsDialog;
}

class RecordDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecordDetailsDialog(const QVariant &id, QWidget *parent = nullptr);
    ~RecordDetailsDialog();

    int exec() override;

    QString getNumber() const;
    QString getName() const;

public slots:
    void reject() override;

private:
    Ui::RecordDetailsDialog *ui;
    QDataWidgetMapper *m_mapper = nullptr;

    QSqlTableModel m_model;
    QVariant m_id;

    bool validate();
};

#endif // RECORDDETAILSDIALOG_H
