#ifndef AUTHORITYDETAILSDIALOG_H
#define AUTHORITYDETAILSDIALOG_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QVariant>

namespace Ui {
class AuthorityDetailsDialog;
}

class AuthorityDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorityDetailsDialog(const QVariant &id, QWidget *parent = nullptr);
    ~AuthorityDetailsDialog();

    int exec() override;

    QString getName() const;

public slots:
    void reject() override;

private:
    Ui::AuthorityDetailsDialog *ui;
    QDataWidgetMapper *m_mapper;

    QVariant m_id;

    bool validate();
};

#endif // AUTHORITYDETAILSDIALOG_H
