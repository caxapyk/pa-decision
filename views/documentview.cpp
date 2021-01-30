#include "documentview.h"
#include "application.h"
#include "dialogs/documentformdialog.h"

#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

DocumentView::DocumentView(QWidget *parent) : TableWidgetView(parent)
{
    setFrameStyle(QFrame::NoFrame);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setSortingEnabled(false);
    horizontalHeader()->setSortIndicatorShown(true);
    setColumnCount(m_headerLabels.length());

    setHorizontalHeaderLabels(m_headerLabels);
    hideColumn(0); //id
    horizontalHeader()->setSectionResizeMode(m_headerLabels.length() - 1, QHeaderView::Stretch);

    connect(horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &DocumentView::sort);

    // save header state dynamicly
    connect(horizontalHeader(), &QHeaderView::sectionResized, [=] { saveViewState(); } );
    connect(horizontalHeader(), &QHeaderView::sortIndicatorChanged, [=] { saveViewState(); } );
}

DocumentView::~DocumentView()
{

}

void DocumentView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    horizontalHeader()->restoreState(settings->value("Views/DocumentView").toByteArray());

    m_order = horizontalHeader()->sortIndicatorOrder();
}

void DocumentView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("DocumentView", horizontalHeader()->saveState());
    settings->endGroup();
}

void DocumentView::contextMenu(BaseContextMenu &menu)
{
    menu.addAction("Open");
}

void DocumentView::refresh()
{
    if (!m_authorityId.isValid())
        return;

    _clear();

    QString fieldName;
    switch (horizontalHeader()->sortIndicatorSection()) {
    case 1: fieldName = "cast(number as unsigned)"; break;
    case 2: fieldName = "name"; break;
    case 3: fieldName = "comment"; break;

    default: fieldName = "id"; break;
    }

    QString sort = fieldName + (m_order == Qt::AscendingOrder ? " asc" : " desc");

    QSqlQuery query;
    query.prepare(QString("select id, number, name, comment from pad_decision where authority_id=? order by %1").arg(sort));
    query.bindValue(0, m_authorityId);
    query.exec();

    if(query.isActive()) {
        if(!query.size())
            return;

        int cols = query.record().count();

        while(query.next()) {
            setRowCount(rowCount() + 1);

            int row = query.at();
             for (int j = 0; j < cols; ++j) {
                QTableWidgetItem *item = new QTableWidgetItem(query.value(j).toString());
                setItem(row, j, item);
             }
        }

        hideColumn(0); //id
    } else {
        qDebug() << query.lastError().text();

        QMessageBox::warning(this,
                tr("Documents"),
                tr("Could not fetch data."),
                QMessageBox::Ok);
    }

    updateTotal();
}

void DocumentView::editRow()
{
    int row = selectedRanges().last().bottomRow();
    const QVariant id = item(row, 0)->data(Qt::DisplayRole);

    DocumentFormDialog dialog;
    dialog.setAuthorityId(m_authorityId);
    dialog.setId(id);
    int res = dialog.exec();

    if(res == DocumentFormDialog::Accepted) {
        int row = currentRow();
        item(row, 0)->setText(dialog.getId().toString());
        item(row, 1)->setText(dialog.getNumber().toString());
        item(row, 2)->setText(dialog.getTitle().toString());
        item(row, 3)->setText(dialog.getComment().toString());
    }
}

void DocumentView::_insertRow()
{
    DocumentFormDialog dialog;
    dialog.setAuthorityId(m_authorityId);
    int res = dialog.exec();

    if(res == DocumentFormDialog::Accepted) {
        insertRow(0);

        setItem(0, 0, new QTableWidgetItem(dialog.getId().toString()));
        setItem(0, 1, new QTableWidgetItem(dialog.getNumber().toString()));
        setItem(0, 2, new QTableWidgetItem(dialog.getTitle().toString()));
        setItem(0, 3, new QTableWidgetItem(dialog.getComment().toString()));

        selectRow(0);
    }

    emit totalChanged(m_total + 1);
}

void DocumentView::removeRows()
{
    const QList<QTableWidgetSelectionRange> range = selectedRanges();

    int res = QMessageBox::critical(this,
        tr("Documents"),
        tr("Are you shure that you want to delete %1 item(s)?").arg(range.length()),
        QMessageBox::No | QMessageBox::Yes);

       if (res == QMessageBox::Yes) {
           QSqlQuery query;
           query.prepare("delete from pad_decision where id=?");

           for (int i = 0; i < range.length(); ++i) {
                int row = range.at(i).topRow() - i;

                query.bindValue(0, item(row, 0)->text());

                if(query.exec()) {
                    removeRow(row);
                    emit totalChanged(m_total - 1);
                } else {
                    qDebug() << query.lastError().text();
                    QMessageBox::warning(this,
                            tr("Documents"),
                            tr("Could not remove the items."),
                            QMessageBox::Ok);
                }

                query.finish();
            }
    }
}

void DocumentView::sort(int, Qt::SortOrder order)
{
    m_order = order;
    refresh();
}

void DocumentView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    TableWidgetView::selectionChanged(selected, deselected);

    application->mainWindow()->insertAction()->setEnabled(isInsertEnabled());
    application->mainWindow()->editAction()->setEnabled(isEditEnabled());
    application->mainWindow()->removeAction()->setEnabled(isRemoveEnabled());
    application->mainWindow()->refreshAction()->setEnabled(isRefreshEnabled());

    emit _selected(selectedRanges());
}

void DocumentView::updateTotal()
{
    QSqlQuery query;
    query.prepare("select count(id) from pad_decision where authority_id=?");
    query.bindValue(0, m_authorityId);

    query.exec();
    if(query.isActive()) {
        query.first();

        m_total = query.value("count(id)").toInt();
        emit totalChanged(m_total);
    }
}
