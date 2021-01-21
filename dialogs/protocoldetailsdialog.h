#ifndef PROTOCOLDETAILSDIALOG_H
#define PROTOCOLDETAILSDIALOG_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QSqlRecord>
#include <QVariant>

namespace Ui {
class ProtocolDetailsDialog;
}

class ProtocolDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProtocolDetailsDialog(const QVariant &id = QVariant(), QWidget *parent = nullptr);
    ~ProtocolDetailsDialog();

    int exec() override;

    QString getNumber() const;
    QDate getDate() const;
    int getPages() const;
    QString getName() const;
    QString getComment() const;

public slots:
    void accept() override;
    void reject() override;

private:
    Ui::ProtocolDetailsDialog *ui;
    QDataWidgetMapper *m_mapper = nullptr;

    QVariant m_id;
    bool dateChanged = false;
};

#endif // PROTOCOLDETAILSDIALOG_H
