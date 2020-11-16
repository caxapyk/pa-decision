#ifndef PROTOCOLDIALOG_H
#define PROTOCOLDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/protocolmodel.h"
#include "widgets/dialogheader.h"

#include <QDialog>
#include <QPushButton>
#include <QSortFilterProxyModel>

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

private:
    ProtocolModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    QPushButton *pB_comment;
    QPushButton *pB_details;
    DialogHeader *m_headerWidget;

private slots:
    void details();
    void editComment();
    void loadByAuthorityId(int id);

protected:
    bool choiceButtonEnabled() override;

protected slots:
    virtual void selected(const QModelIndex &current, const QModelIndex &) override;
    virtual int choice(const QModelIndex &current) const override;

};

#endif // PROTOCOLDIALOG_H
