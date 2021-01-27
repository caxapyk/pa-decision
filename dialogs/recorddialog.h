#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include "dialogs/treedialog.h"
#include "models/recordmodel.h"
#include "models/recordproxymodel.h"

#include <QDialog>
#include <QPushButton>
#include <QLabel>

class RecordDialog : public TreeDialog
{
    Q_OBJECT

public:
    explicit RecordDialog(const QVariant &authorityId, const QVariant &fundId = QVariant(), const QVariant &inventoryId = QVariant(), QWidget *parent = nullptr);
    ~RecordDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

    void setChoiceLevel(RecordModel::Levels level);
    RecordModel::Levels choiceLevel() const { return m_choiceLevel; };

    int exec() override;

public slots:
    void insert() override;
    void remove() override;

private:
    RecordModel *m_model;
    RecordProxyModel *m_proxyModel;

    QPushButton *pB_details;
    QPushButton *pB_protocol;

    RecordModel::Levels m_choiceLevel = RecordModel::RecordLevel;

    bool choiceButtonEnabled() override;

private slots:
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous) override;
    QVariant choice(const QModelIndex &current) const override;

    void details();
    void protocols();
};

#endif // RECORDDIALOG_H
