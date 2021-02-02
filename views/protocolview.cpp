#include "protocolview.h"
#include "application.h"
#include "dialogs/protocoldetailsdialog.h"

#include <QDate>
#include <QHeaderView>
#include <QMessageBox>

ProtocolView::ProtocolView(QWidget *parent) : TreeView(parent)
{
    m_model = new ProtocolModel;
    m_proxyModel = new QSortFilterProxyModel;

    restoreViewState();

    m_model->setEditStrategy(ProtocolModel::OnFieldChange);
    m_proxyModel->setSourceModel(m_model);

    setModel(m_proxyModel);

    hideColumn(0);
    hideColumn(1);
    hideColumn(3);
    hideColumn(6);
}

ProtocolView::~ProtocolView()
{
    delete m_model;
    delete m_proxyModel;
}

void ProtocolView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    header()->restoreState(settings->value("Views/ProtocolView").toByteArray());
}

void ProtocolView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("ProtocolView", header()->saveState());
    settings->endGroup();
}

void ProtocolView::details()
{
    QModelIndex index = currentIndex();
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

void ProtocolView::insertRow()
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

            selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
            setCurrentIndex(topLeft);
            scrollTo(topLeft);
        } else {
            QMessageBox::critical(this,
                    tr("Protocols"),
                    tr("Could not create item."),
                    QMessageBox::Ok);
        }
    }
}

void ProtocolView::removeRows()
{
    int res = QMessageBox::critical(this,
        tr("Protocols"),
        tr("Are you shure that you want to delete this item(s)?"),
            QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        const QModelIndexList &indexes = selectionModel()->selectedRows();

        for (int i = 0; i < indexes.count(); ++i) {
            const QModelIndex index = indexes.at(i);
            QModelIndex parent = m_proxyModel->parent(index);

            bool remove = m_proxyModel->removeRow(index.row(), parent);
            if (remove) {
                refresh();
            } else {
                QMessageBox::warning(this,
                tr("Protocols"),
                tr("Could not remove the items."),
                    QMessageBox::Ok);
            }
        }
    }
}

void ProtocolView::refresh()
{
    ProtocolModel *model = dynamic_cast<ProtocolModel*>(m_proxyModel->sourceModel());
    if(model)
        model->select();
}
