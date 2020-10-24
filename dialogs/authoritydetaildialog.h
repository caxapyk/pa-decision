#ifndef AUTHORITYDIALOG_H
#define AUTHORITYDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>

namespace Ui {
class AuthorityDetailDialog;
}

class AuthorityDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorityDetailDialog(QVariant id, QWidget *parent = nullptr);
    ~AuthorityDetailDialog();

public slots:
    void reject() override;

private:
    Ui::AuthorityDetailDialog *ui;

    QSqlTableModel *m_model;
    QDataWidgetMapper *m_mapper;
};

#endif // AUTHORITYDIALOG_H
