#ifndef REFERENCEDIALOG_H
#define REFERENCEDIALOG_H

#include "models/referencemodel.h"

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

    QMap<int, QString> currentChoice() { return m_choice; };
    bool isChoiceMode() { return choice_mode; };
    void setChoiceMode() { choice_mode = true; };
    void setComment(const QString &text);

public slots:
    virtual void edit();
    virtual void insert();
    virtual void refresh();
    virtual void remove();

protected:
    Ui::ReferenceDialog *ui;

    QShortcut *insertShortcut = nullptr;
    QShortcut *editShortcut = nullptr;
    QShortcut *removeShortcut = nullptr;
    QShortcut *refreshShortcut = nullptr;

    QPushButton *commentButton() { return pB_comment; };
    QString inputDialog(const QString &title, const QString &label = QString(), const QString &value = QString());

    void setDialogModel(QAbstractProxyModel *model);

    virtual bool choiceButtonEnabled() { return true; };

protected slots:
    virtual void selected(const QModelIndex &current, const QModelIndex &) = 0;
    virtual QMap<int, QString> choice(const QModelIndex &current) = 0;

private:
    void setupShortcuts();

    bool choice_mode = false;
    QMap<int, QString> m_choice = QMap<int, QString>();

    QAbstractProxyModel *m_dialogProxyModel = nullptr;
    QPushButton *pB_comment = nullptr;

private slots:
    void _selected(const QModelIndex &current, const QModelIndex &);
    void editComment();

    void accept() override;
    void reject() override;
    void closeEvent(QCloseEvent *event) override;

};

#endif // REFERENCEDIALOG_H
