#ifndef PROTOCOLDIALOG_H
#define PROTOCOLDIALOG_H

#include "dialogs/choicedialog.h"
#include "views/protocolview.h"

#include <QPushButton>

class ProtocolDialog : public ChoiceDialog
{
    Q_OBJECT

public:
    explicit ProtocolDialog(QWidget *parent = nullptr);
    ~ProtocolDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

    int exec() override;

private:
    QPushButton *pB_details;
    ProtocolView *m_view;

    bool choiceButtonEnabled() override;

private slots:
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous) override;
    QVariant choice(const QModelIndex &current) const override;
};

#endif // PROTOCOLDIALOG_H
