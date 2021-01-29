#ifndef AFDIALOG_H
#define AFDIALOG_H

#include "dialogs/choicedialog.h"
#include "models/aftreemodel.h"
#include "views/afview.h"

#include <QDialog>
#include <QPushButton>

class AFDialog : public ChoiceDialog
{
    Q_OBJECT

public:
    explicit AFDialog(QWidget *parent = nullptr);
    ~AFDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

    void setChoiceLevel(AFTreeModel::Levels level);
    AFTreeModel::Levels choiceLevel() const { return m_choiceLevel; };

    int exec() override;

private:
    QPushButton *pB_details;
    QPushButton *pB_protocol;
    QPushButton *pB_fund;

    AFView *m_view;

    AFTreeModel::Levels m_choiceLevel = AFTreeModel::RecordLevel;

    bool choiceButtonEnabled() override;

private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
    QVariant choice(const QModelIndex &current) const override;
};

#endif // AFDIALOG_H
