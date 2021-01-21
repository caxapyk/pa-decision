#ifndef REFERENCEDIALOG_H
#define REFERENCEDIALOG_H

#include "models/referencemodel.h"

#include <QDialog>
#include <QShortcut>
#include <QPushButton>
#include <QSortFilterProxyModel>

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
    virtual bool choiceButtonEnabled() { return true; };

    void contextMenu(const QPoint &pos);

    void clearInfoText();
    void clearSelection();

    int currentChoice() { return m_choice; };
    bool isChoiceMode() { return choice_mode; };
    QVariant inputDialog(const QString &title, const QString &label = QString(), const QVariant &value = QVariant());

    void setChoiceMode() { choice_mode = true; };
    void setInfoText(const QString &text);
    void setInfoIconVisible(bool ok = true);

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

    void setDialogModel(QSortFilterProxyModel *model);

protected slots:
    virtual void selected(const QItemSelection &selected, const QItemSelection &deselected);
    virtual int choice(const QItemSelection &selected) const = 0;

private:
    void setupShortcuts();

    bool choice_mode = false;
    int m_choice = -1;

    QSortFilterProxyModel *m_dialogProxyModel = nullptr;

private slots:
    void _selected(const QItemSelection &selected, const QItemSelection &deselected);

    void accept() override;
    void reject() override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // REFERENCEDIALOG_H
