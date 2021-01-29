#ifndef FUNDDETAILSDIALOG_H
#define FUNDDETAILSDIALOG_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QVariant>

namespace Ui {
class FundDetailsDialog;
}

class FundDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FundDetailsDialog(const QVariant &id, QWidget *parent = nullptr);
    ~FundDetailsDialog();

    int exec() override;

    QString getNumber() const;
    QString getName() const;

public slots:
    void reject() override;

private:
    Ui::FundDetailsDialog *ui;
    QDataWidgetMapper *m_mapper = nullptr;

    QVariant m_id;

    bool validate();
};

#endif // FUNDDETAILSDIALOG_H
