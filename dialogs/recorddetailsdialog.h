#ifndef RECORDDETAILSDIALOG_H
#define RECORDDETAILSDIALOG_H

#include "dialogs/detailsdialog.h"

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>

namespace Ui {
class RecordDetailsDialog;
}

class RecordDetailsDialog : public DetailsDialog
{
    Q_OBJECT

public:
    explicit RecordDetailsDialog(QVariant id, QWidget *parent = nullptr);
    ~RecordDetailsDialog();

public slots:
    void reject() override;

private:
    Ui::RecordDetailsDialog *ui;

    QSqlTableModel *m_model;
    QDataWidgetMapper *m_mapper;
};

#endif // RECORDDETAILSDIALOG_H
