#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/authorityflatmodel.h"
#include "models/recordmodel.h"
#include "models/recordproxymodel.h"
#include "widgets/dialogheader.h"

#include <QDialog>
#include <QPushButton>
#include <QLabel>

class RecordDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit RecordDialog(QWidget *parent = nullptr);
    ~RecordDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

    int exec() override;

    void setAuthorityId(int id) { m_authorityId = id; };

public slots:
    void insert() override;
    void remove() override;

private:
    RecordModel *m_model;
    RecordProxyModel *m_proxyModel;

    QPushButton *pB_details;
    DialogHeader *m_headerWidget = nullptr;

    int m_authorityId = -1;

protected:
    bool choiceButtonEnabled() override;

protected slots:
    virtual void selected(const QItemSelection &selected, const QItemSelection &deselected) override;
    virtual int choice(const QItemSelection &selected) const override;

private slots:
    void details();
};

#endif // RECORDDIALOG_H
