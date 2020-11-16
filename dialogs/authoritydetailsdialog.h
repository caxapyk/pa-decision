#ifndef AUTHORITYDETAILSDIALOG_H
#define AUTHORITYDETAILSDIALOG_H

#include "dialogs/detailsdialog.h"

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>

namespace Ui {
class AuthorityDetailsDialog;
}

class AuthorityDetailsDialog : public DetailsDialog
{
    Q_OBJECT

public:
    explicit AuthorityDetailsDialog(QVariant id, QWidget *parent = nullptr);
    ~AuthorityDetailsDialog();

public slots:
    void reject() override;

private:
    Ui::AuthorityDetailsDialog *ui;

    QSqlTableModel *m_model;
    QDataWidgetMapper *m_mapper;
};

#endif // AUTHORITYDETAILSDIALOG_H
