#ifndef CHOICEDIALOG_H
#define CHOICEDIALOG_H

#include "views/treeview.h"

#include <QDialog>
#include <QPushButton>
#include <QBoxLayout>

namespace Ui {
class ChoiceDialog;
}

class ChoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChoiceDialog(QWidget *parent = nullptr);
    ~ChoiceDialog();

    TreeView *treeView() { return m_view; };
    void setTreeView(TreeView *tv);

    QVariant currentChoice() const { return m_choice; };
    bool isChoiceMode() { return choice_mode; };
    void setChoiceMode(bool ok = true);

    void setInfoText(const QString &text);
    void clearInfoText();

    void setInfoIconVisible(bool ok = true);

    QBoxLayout *buttonLayout();

    virtual void restoreDialogState() {};
    virtual void saveDialogState() {};
    virtual bool choiceButtonEnabled() { return true; };

protected slots:
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    virtual QVariant choice(const QModelIndex &) const { return QVariant(); };

private:
    Ui::ChoiceDialog *ui;
    TreeView *m_view;

    bool choice_mode = false;
    QVariant m_choice;

private slots:
    void accept() override;
    void reject() override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // CHOICEDIALOG_H
