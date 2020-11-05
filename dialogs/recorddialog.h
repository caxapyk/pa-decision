#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/recordmodel.h"
#include "models/recordproxymodel.h"

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

    //void setChooseMode(bool ok) { chr_mode = ok; };
    //QMap<int, QString> current() { return m_current; };

public slots:
    void edit() override;
    void insert() override;
    void refresh() override;
    void remove() override;

private:
    RecordModel *m_model;
    RecordProxyModel *m_proxyModel;

    QPushButton *pB_fundTitle;

    //bool chr_mode = false;
    //QMap<int, QString> m_current = QMap<int, QString>();

    void setInfoText();

private slots:
    void changeCurrent(const QModelIndex &current, const QModelIndex &) override;
    void editFundName();
};

#endif // RECORDDIALOG_H
