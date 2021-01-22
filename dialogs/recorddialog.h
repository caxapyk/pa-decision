#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include "dialogs/treedialog.h"
#include "models/recordmodel.h"
#include "models/recordproxymodel.h"
#include "widgets/dialogheader.h"

#include <QDialog>
#include <QPushButton>
#include <QLabel>

class RecordDialog : public TreeDialog
{
    Q_OBJECT

public:
    explicit RecordDialog(QWidget *parent = nullptr);
    ~RecordDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

    int exec() override;

    void setAuthorityId(const QVariant &id) { m_authorityId = id; };

public slots:
    void insert() override;
    void remove() override;

private:
    RecordModel *m_model;
    RecordProxyModel *m_proxyModel;

    QPushButton *pB_details;
    QPushButton *pB_protocol;
    QVariant m_authorityId;

protected:
    bool choiceButtonEnabled() override;

protected slots:
    virtual void selected(const QItemSelection &selected, const QItemSelection &deselected) override;
    virtual int choice(const QItemSelection &selected) const override;

private slots:
    void details();
    void protocols();
};

#endif // RECORDDIALOG_H
