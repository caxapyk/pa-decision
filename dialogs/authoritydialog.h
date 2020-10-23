#ifndef AUTHORITYDIALOG_H
#define AUTHORITYDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>

namespace Ui {
class AuthorityDialog;
}

class AuthorityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthorityDialog(QVariant id, QWidget *parent = nullptr);
    ~AuthorityDialog();

public slots:
    void reject() override;

private:
    Ui::AuthorityDialog *ui;

    QSqlTableModel *m_model;
    QDataWidgetMapper *m_mapper;
};

#endif // AUTHORITYDIALOG_H
