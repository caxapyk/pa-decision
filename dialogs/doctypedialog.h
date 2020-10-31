#ifndef DOCTYPEDIALOG_H
#define DOCTYPEDIALOG_H

#include "dialogs/referencedialog.h"
#include "models/doctypemodel.h"

#include <QDialog>
#include <QShortcut>
#include <QSortFilterProxyModel>

class DoctypeDialog : public ReferenceDialog
{
    Q_OBJECT

public:
    explicit DoctypeDialog(QWidget *parent = nullptr);
    ~DoctypeDialog();

    void restoreDialogState() override;
    void saveDialogState() override;

public slots:
    void edit() override;
    void insert() override;
    void refresh() override;
    void remove() override;

private:
    DoctypeModel *m_model;
    QSortFilterProxyModel *m_proxyModel;

private slots:
    void changeCurrent(const QModelIndex &current, const QModelIndex &) override;
};

#endif // DOCTYPEDIALOG_H
