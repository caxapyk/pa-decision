#ifndef PROTOCOLDIALOG_H
#define PROTOCOLDIALOG_H

#include "dialogs/treedialog.h"
#include "models/protocolmodel.h"

#include <QDataWidgetMapper>
#include <QPushButton>
#include <QSortFilterProxyModel>

class ProtocolDialog : public TreeDialog
{
    Q_OBJECT

public:
    explicit ProtocolDialog(const QVariant &recordId = QVariant(), QWidget *parent = nullptr);
    ~ProtocolDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

public slots:
    void insert() override;

private:
    ProtocolModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

    QPushButton *pB_details;

    bool choiceButtonEnabled() override;

private slots:
    void onCurrentChanged(const QModelIndex &current, const QModelIndex &previous) override;
    QVariant choice(const QModelIndex &current) const override;

    void details();
};

#endif // PROTOCOLDIALOG_H
