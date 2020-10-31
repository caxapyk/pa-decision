#ifndef REFERENCEDIALOG_H
#define REFERENCEDIALOG_H

#include <QDialog>
#include <QShortcut>

namespace Ui {
class ReferenceDialog;
}

class ReferenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReferenceDialog(QWidget *parent = nullptr);
    ~ReferenceDialog();

    virtual void restoreDialogState() {};
    virtual void saveDialogState() {};

    void contextMenu(const QPoint &pos);

public slots:
    virtual void edit() {};
    virtual void insert() {};
    virtual void refresh() {};
    virtual void remove() {};

protected:
    Ui::ReferenceDialog *ui;

    QShortcut *insertShortcut = nullptr;
    QShortcut *editShortcut = nullptr;
    QShortcut *removeShortcut = nullptr;
    QShortcut *refreshShortcut = nullptr;

    void closeEvent(QCloseEvent *event) override;

private:
    void setupShortcuts();

private slots:
    virtual void changeCurrent(const QModelIndex &current, const QModelIndex &) {};

    void accept() override;
    void reject() override;
};

#endif // REFERENCEDIALOG_H
