#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/recordtreemodel.h"
#include "models/recordproxymodel.h"

#include <QDialog>
#include <QPushButton>

namespace Ui {
class RecordDialog;
}

class RecordDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit RecordDialog(QWidget *parent = nullptr);
    ~RecordDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

public slots:
    void edit() override;
    void insert() override;
    void refresh() override;
    void remove() override;

private:
    RecordTreeModel *m_model;
    RecordProxyModel *m_proxyModel;

    QPushButton *pB_comment;
    QPushButton *pB_fundTitle;

    void setInfoText();

private slots:
    void changeCurrent(const QModelIndex &current, const QModelIndex &) override;
    void editComment();
    void editFundName();
};

#endif // RECORDDIALOG_H
