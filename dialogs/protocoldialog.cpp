#include "protocoldialog.h"

#include "application.h"
#include "dialogs/protocoldetailsdialog.h"

#include <QDebug>
#include <QDate>
#include <QInputDialog>
#include <QItemSelection>
#include <QMenu>
#include <QMessageBox>
#include <QHeaderView>

ProtocolDialog::ProtocolDialog(const QVariant &recordId, QWidget *parent) :
    ChoiceDialog(parent)
{
    restoreDialogState();

    setWindowTitle(tr("Protocols"));

    pB_details = new QPushButton(tr("Details"));
    pB_details->setDisabled(true);

    buttonLayout()->addWidget(pB_details);

    connect(pB_details, &QPushButton::clicked, this, &ProtocolDialog::details);

    m_model = new ProtocolModel;
    m_model->setEditStrategy(ProtocolModel::OnFieldChange);
    if(recordId.isValid()) {
        m_model->setRecordId(recordId);
    }
    m_model->select();

    m_proxyModel = new QSortFilterProxyModel;
    m_proxyModel->setSourceModel(m_model);

    treeView()->setModel(m_proxyModel);
    treeView()->hideColumn(0);
    treeView()->hideColumn(1);
    treeView()->hideColumn(3);
    treeView()->hideColumn(6);

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
    treeView()->header()->restoreState(settings->value("ProtocolDialog/tV_itemView").toByteArray());
}

void ProtocolDialog::saveDialogState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("ProtocolDialog");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("tV_itemView", treeView()->header()->saveState());
    settings->endGroup();
}

void ProtocolDialog::onCurrentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    pB_details->setEnabled(current.isValid());
    ChoiceDialog::onCurrentChanged(current, previous);
}

void ProtocolDialog::details()
{
    QModelIndex index = treeView()->currentIndex();
    QVariant id = index.siblingAtColumn(0).data();

    ProtocolDetailsDialog dialog(id);
    int res = dialog.exec();

    if(res == ProtocolDetailsDialog::Accepted) {
        m_proxyModel->setData(index.siblingAtColumn(2), dialog.getNumber());
        m_proxyModel->setData(index.siblingAtColumn(3), dialog.getPages());
        m_proxyModel->setData(index.siblingAtColumn(4), dialog.getName());
        m_proxyModel->setData(index.siblingAtColumn(5), dialog.getDate());
        m_proxyModel->setData(index.siblingAtColumn(6), dialog.getComment());
    }
}

bool ProtocolDialog::choiceButtonEnabled()
{
    return !isChoiceMode() || !treeView()->selectionModel()->selection().isEmpty();
}

QVariant ProtocolDialog::choice(const QModelIndex &current) const
{
    if(current.isValid()) {
        return m_proxyModel->mapToSource(current.siblingAtColumn(0)).data();
    }

    return QVariant();
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

            treeView()->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
            treeView()->setCurrentIndex(topLeft);
            treeView()->scrollTo(topLeft);
        } else {
            QMessageBox::critical(this,
                    tr("Protocols"),
                    tr("Could not create item."),
                    QMessageBox::Ok);
        }
    }
}
