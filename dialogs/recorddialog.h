#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/recordmodel.h"
#include "models/recordproxymodel.h"
#include "widgets/recorddialogheader.h"

#include <QDialog>
#include <QPushButton>

class RecordDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit RecordDialog(QWidget *parent = nullptr);
    ~RecordDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

private:
    RecordModel *m_model;
    RecordProxyModel *m_proxyModel;

    QPushButton *pB_fundTitle;
    RecordDialogHeader *m_headerWidget;

protected slots:
    virtual void selected(const QModelIndex &current, const QModelIndex &) override;
    virtual QMap<int, QString> choice(const QModelIndex &current) override;

private slots:
    void editFundName();
};

#endif // RECORDDIALOG_H
