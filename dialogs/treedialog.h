#ifndef REFERENCEDIALOG_H
#define REFERENCEDIALOG_H

#include "models/sqlbasemodel.h"
#include "utils/customcontextmenu.h"
#include "widgets/customtreeview.h"

#include <QDialog>
#include <QShortcut>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QBoxLayout>

namespace Ui {
class TreeDialog;
}

class TreeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TreeDialog(QWidget *parent = nullptr);
    ~TreeDialog();

    virtual void restoreDialogState() {};
    virtual void saveDialogState() {};
    virtual bool choiceButtonEnabled() { return true; };

    void clearInfoText();
    void clearSelection();

    int currentChoice() { return m_choice; };
    bool isChoiceMode() { return choice_mode; };
    QVariant inputDialog(const QString &title, const QString &label = QString(), const QVariant &value = QVariant());

    void setChoiceMode() { choice_mode = true; };
    void setInfoText(const QString &text);
    void setInfoIconVisible(bool ok = true);

    QBoxLayout *buttonLayout();

public slots:
    virtual void edit();
    virtual void insert();
    virtual void refresh();
    virtual void remove();

protected:
    CustomTreeView *m_tree;

    void setProxyModel(QSortFilterProxyModel *model);

protected slots:
    virtual void selected(const QItemSelection &selected, const QItemSelection &deselected);
    virtual int choice(const QItemSelection &selected) const = 0;

private:
    Ui::TreeDialog *ui;

    QSortFilterProxyModel *m_dialogProxyModel = nullptr;

    bool choice_mode = false;
    int m_choice = -1;

private slots:
    void contextMenu(CustomContextMenu &menu);
    void _selected(const QItemSelection &selected, const QItemSelection &deselected);

    void accept() override;
    void reject() override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // REFERENCEDIALOG_H
