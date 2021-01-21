#include "protocoldialog.h"
#include "ui_treedialog.h"

#include "application.h"
#include "dialogs/protocoldetailsdialog.h"

#include <QDebug>
#include <QDate>
#include <QInputDialog>
#include <QItemSelection>
#include <QMenu>
#include <QMessageBox>

ProtocolDialog::ProtocolDialog(const QVariant &recordId, QWidget *parent) :
    TreeDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Protocols"));

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    ui->vL_buttonGroup->addWidget(pB_details);

    connect(pB_details, &QPushButton::clicked, this, &ProtocolDialog::details);

    m_model = new ProtocolModel;
    if(recordId.isValid()) {
        m_model->setRecordId(recordId);
    }
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    ui->tV_itemView->setModel(m_proxyModel);
    ui->tV_itemView->hideColumn(0);
    ui->tV_itemView->hideColumn(6);

    setProxyModel(m_proxyModel);
}

ProtocolDialog::~ProtocolDialog()
{
    saveDialogState();

    delete pB_details;

    delete m_model;
    delete m_proxyModel;
}

void ProtocolDialog::restoreDialogState()
{
    QSettings* settings = application->applicationSettings();

    restoreGeometry(settings->value("ProtocolDialog/geometry").toByteArray());
    ui->tV_itemView->header()->restoreState(settings->value("ProtocolDialog/tV_itemView").toByteArray());
}

void ProtocolDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("ProtocolDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", ui->tV_itemView->header()->saveState());
    settings->endGroup();
}

int ProtocolDialog::exec()
{
    if(m_authorityId.isValid()) {
        clearInfoText();

        //m_model->setAuthorityId(m_authorityId.toInt());
        m_model->select();

        clearSelection();
    }

    return TreeDialog::exec();
}

void ProtocolDialog::selected(const QItemSelection &selected, const QItemSelection &deselected)
{
    pB_details->setEnabled(!selected.isEmpty());

    TreeDialog::selected(selected, deselected);
}

void ProtocolDialog::details()
{
    QModelIndex index = ui->tV_itemView->currentIndex();
    QVariant id = index.siblingAtColumn(0).data();

    ProtocolDetailsDialog dialog(id);
    int res = dialog.exec();

    if(res == ProtocolDetailsDialog::Accepted) {
        qDebug() << index.siblingAtColumn(2).data() << dialog.getNumber();
        m_proxyModel->setData(index.siblingAtColumn(2), dialog.getNumber());
        m_proxyModel->setData(index.siblingAtColumn(3), dialog.getPages());
        m_proxyModel->setData(index.siblingAtColumn(4), dialog.getName());
        m_proxyModel->setData(index.siblingAtColumn(5), dialog.getDate());
        m_proxyModel->setData(index.siblingAtColumn(6), dialog.getComment());
    }
}

bool ProtocolDialog::choiceButtonEnabled()
{
    return !isChoiceMode() || !ui->tV_itemView->selectionModel()->selection().isEmpty();
}

int ProtocolDialog::choice(const QItemSelection &selected) const
{
    if(!selected.isEmpty()) {
        QModelIndex current = selected.indexes().at(0).siblingAtColumn(0);
        return m_proxyModel->mapToSource(current).data().toInt();
    }

    return -1;
}

void ProtocolDialog::insert()
{
    ProtocolDetailsDialog dialog;
    int res = dialog.exec();

    if(res == ProtocolDetailsDialog::Accepted) {
        QSqlRecord record = m_model->record();

        record.remove(0);
        record.setValue("record_id", m_model->recordId());
        record.setGenerated("record_id", true);
        record.setValue("number", dialog.getNumber());
        record.setGenerated("number", true);
        record.setValue("pages", dialog.getPages());
        record.setGenerated("pages", true);
        record.setValue("name", dialog.getName());
        record.setGenerated("name", true);
        record.setValue("date", dialog.getDate());
        record.setGenerated("date", true);
        record.setValue("comment", dialog.getComment());
        record.setGenerated("comment", true);

        if(m_model->insertRecord(0, record)) {
            QModelIndex topLeft = m_proxyModel->mapFromSource(
                        m_proxyModel->sourceModel()->index(0, 0));
            QModelIndex bottomRight = m_proxyModel->mapFromSource(
                        m_proxyModel->sourceModel()->index(0, m_proxyModel->columnCount() - 1));

            QItemSelection selection(topLeft, bottomRight);
            ui->tV_itemView->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
            ui->tV_itemView->scrollTo(topLeft);
        } else {
            QMessageBox::critical(this,
                    tr("New protocol"),
                    tr("Could not create new protocol"),
                    QMessageBox::Ok);
        }
    }
}
