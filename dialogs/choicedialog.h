#ifndef REFERENCEDIALOG_H
#define REFERENCEDIALOG_H

//#include "widgets/basecontextmenu.h"
//#include "widgets/customtreeview.h"
#include "views/treeview.h"

#include <QDialog>
#include <QShortcut>
#include <QPushButton>
#include <QSortFilterProxyModel>
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

    virtual void restoreDialogState() {};
    virtual void saveDialogState() {};
    virtual bool choiceButtonEnabled() { return true; };

    void clearInfoText();
    //void setProxyModel(QSortFilterProxyModel *model);
    //void clearSelection();

    //CustomTreeView *treeView() { return m_tree; };
    TreeView *treeView() { return m_treeView; };
    void setTreeView(TreeView *tv);

    QVariant currentChoice() const { return m_choice; };
    bool isChoiceMode() { return choice_mode; };
    void setChoiceMode(bool ok = true);

    void setInfoText(const QString &text);
    void setInfoIconVisible(bool ok = true);

    QBoxLayout *buttonLayout();

/*public slots:
    virtual void edit();
    virtual void insert();
    virtual void refresh();
    virtual void remove();*/

protected slots:
    virtual void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous);
    virtual QVariant choice(const QModelIndex &) const { return QVariant(); };

private:
    Ui::ChoiceDialog *ui;
    //CustomTreeView *m_tree;
    TreeView *m_treeView;

    //QSortFilterProxyModel *m_dialogProxyModel = nullptr;

    bool choice_mode = false;
    QVariant m_choice;

private slots:
    void accept() override;
    void reject() override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // REFERENCEDIALOG_H
