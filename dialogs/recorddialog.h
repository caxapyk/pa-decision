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

    void setChoiceLevel(RecordModel::Levels level) { m_choiceLevel = level; };

public slots:
    void insert() override;
    void remove() override;

private:
    RecordModel *m_model;
    RecordProxyModel *m_proxyModel;

    QPushButton *pB_details;
    QPushButton *pB_protocol;

    RecordModel::Levels m_choiceLevel = RecordModel::RecordLevel;

protected:
    bool choiceButtonEnabled() override;

protected slots:
    virtual void selected(const QItemSelection &selected, const QItemSelection &deselected) override;
    virtual QVariant choice(const QItemSelection &selected) const override;

private slots:
    void details();
    void protocols();
};

#endif // RECORDDIALOG_H
