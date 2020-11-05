#ifndef REFERENCEDIALOG_H
#define REFERENCEDIALOG_H

#include <QAbstractItemModel>
#include <QAbstractProxyModel>
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

    QMap<int, QString> currentChoice() { return choice(); };
    bool isChoiceMode() { return choice_mode; };
    void setChoiceMode() { choice_mode = true; };

public slots:
    virtual void edit() {};
    virtual void insert() {};
    virtual void refresh() {};
    virtual void remove() {};

    virtual void editComment();

protected:
    Ui::ReferenceDialog *ui;

    QAbstractProxyModel *m_dialogProxyModel = nullptr;

    QShortcut *insertShortcut = nullptr;
    QShortcut *editShortcut = nullptr;
    QShortcut *removeShortcut = nullptr;
    QShortcut *refreshShortcut = nullptr;

    QPushButton *pB_comment = nullptr;

    void closeEvent(QCloseEvent *event) override;

    virtual QMap<int, QString> choice() {};

    void addCommentButton();
    QPushButton *commentButton() { return pB_comment; };

    void setDialogModel(QAbstractProxyModel *model) { m_dialogProxyModel = model; };

private:
    void setupShortcuts();

private slots:
    virtual void changeCurrent(const QModelIndex &current, const QModelIndex &) = 0;

    void accept() override;
    void reject() override;

    bool choice_mode = false;
    QMap<int, QString> m_choice = QMap<int, QString>();
};

#endif // REFERENCEDIALOG_H
