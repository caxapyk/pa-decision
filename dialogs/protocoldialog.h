#ifndef PROTOCOLDIALOG_H
#define PROTOCOLDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/standardreferencemodel.h"
#include "widgets/dialogheader.h"

#include <QDialog>
#include <QDataWidgetMapper>
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
    void insert() override;

private:
    StandardReferenceModel *m_model;
    QSortFilterProxyModel *m_proxyModel;
    QDataWidgetMapper *m_mapper = nullptr;

    QPushButton *pB_comment;
    QPushButton *pB_details;
    DialogHeader *m_headerWidget;

private slots:
    void details();

protected:
    bool choiceButtonEnabled() override;

protected slots:
    virtual void selected(const QItemSelection &selected, const QItemSelection &deselected) override;
    virtual int choice(const QItemSelection &selected) const override;

};

#endif // PROTOCOLDIALOG_H
