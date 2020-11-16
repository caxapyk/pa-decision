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

public slots:
    void insert() override;
    void remove() override;

private:
    RecordModel *m_model;
    RecordProxyModel *m_proxyModel;

    QPushButton *pB_comment;
    QPushButton *pB_details;
    DialogHeader *m_headerWidget;

protected:
    bool choiceButtonEnabled() override;

protected slots:
    virtual void selected(const QModelIndex &current, const QModelIndex &) override;
    virtual int choice(const QModelIndex &current) const override;

private slots:
    void editComment();
    void details();
    void loadByAuthorityId(int id);
};

#endif // RECORDDIALOG_H
