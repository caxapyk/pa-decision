#ifndef REFERENCEDIALOG_H
#define REFERENCEDIALOG_H

#include <QDialog>

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
    virtual void setupShortcuts() {};

public slots:
    virtual void edit() {};
    virtual void insert() {};
    virtual void refresh() {};
    virtual void remove() {};

protected:
    Ui::ReferenceDialog *ui;

private slots:
    virtual void changeCurrent(const QModelIndex &current, const QModelIndex &) {};
    virtual void contextMenu(const QPoint &pos) {};
};

#endif // REFERENCEDIALOG_H
