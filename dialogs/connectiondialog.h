#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();

private:
    Ui::ConnectionDialog *ui;
    QSqlDatabase test_db;

private slots:
    void testConnection();
    void save();
};

#endif // CONNECTIONDIALOG_H
