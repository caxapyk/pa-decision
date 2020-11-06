#ifndef PROTOCOLDIALOG_H
#define PROTOCOLDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/recordmodel.h"
#include "models/recordproxymodel.h"

#include <QDialog>
#include <QPushButton>

class ProtocolDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit ProtocolDialog(QWidget *parent = nullptr);
    ~ProtocolDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

public slots:
    void edit() override;
    void insert() override;
    void refresh() override;
    void remove() override;

private:
    RecordModel *m_model;
    RecordProxyModel *m_proxyModel;

    QPushButton *pB_comment;

protected slots:
    virtual void selected(const QModelIndex &current, const QModelIndex &) override;
    virtual int choice(const QModelIndex &current) const override;

};

#endif // PROTOCOLDIALOG_H
