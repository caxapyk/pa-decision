#ifndef PROTOCOLDIALOG_H
#define PROTOCOLDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/protocolmodel.h"

#include <QDataWidgetMapper>
#include <QPushButton>
#include <QSortFilterProxyModel>

class ProtocolDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit ProtocolDialog(const QVariant &recordId = QVariant(), QWidget *parent = nullptr);
    ~ProtocolDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

    int exec() override;

    void setAuthorityId(const QVariant &id) { m_authorityId = id; };

public slots:
    void insert() override;

private:
    ProtocolModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    QPushButton *pB_details;

    QVariant m_authorityId;

private slots:
    void details();

protected:
    bool choiceButtonEnabled() override;

protected slots:
    virtual void selected(const QItemSelection &selected, const QItemSelection &deselected) override;
    virtual int choice(const QItemSelection &selected) const override;
};

#endif // PROTOCOLDIALOG_H
