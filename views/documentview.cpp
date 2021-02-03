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

    m_viewAction = new QAction(tr("View"));

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

    restoreViewState();
}

DocumentView::~DocumentView()
{
    delete m_viewAction;
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
    QList<int> rows = selectedRows();

    m_viewAction->setEnabled(rows.count() == 1);
    menu.insertAction(menu.action(BaseContextMenu::Insert), m_viewAction);

    connect(m_viewAction, &QAction::triggered, this, [=] {
        emit viewDocument(rows.last());
    });

    menu.insertSeparator(menu.action(BaseContextMenu::Insert));
}

void DocumentView::refresh()
{
    if (!m_authorityId.isValid())
        return;

    _clear();

    // sorting
    QString fieldName;
    switch (horizontalHeader()->sortIndicatorSection()) {
    case 1: fieldName = "cast(number as unsigned)"; break;
    case 2: fieldName = "name"; break;
    case 3: fieldName = "comment"; break;

    default: fieldName = "id"; break;
    }

    QString sort = fieldName + (m_order == Qt::AscendingOrder ? " asc" : " desc");

    // pagination
    QString limit = QString("limit %1,%2").arg(m_from).arg(m_count);

    QSqlQuery query;
    query.prepare(QString("select id, number, name, comment from pad_decision where authority_id=? order by %1 %2")
                  .arg(sort)
                  .arg(limit));

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

    emit refreshed();
}

void DocumentView::editRow()
{
    int row = selectedRanges().last().bottomRow();
    const QVariant id = item(row, 0)->data(Qt::DisplayRole);

    DocumentFormDialog dialog;
    dialog.setAuthorityId(m_authorityId);
    dialog.setId(id);
    dialog.exec();

    if(dialog.isApplied()) {
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
    dialog.exec();

    if(dialog.isApplied()) {
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
    QList<int> rows = selectedRows();

    int res = QMessageBox::critical(this,
        tr("Documents"),
        tr("Are you shure that you want to delete %1 item(s)?").arg(rows.count()),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
    QSqlQuery query;
    query.prepare("delete from pad_decision where id=?");

        for (int i = 0; i < rows.count(); ++i) {
            int row = rows.at(i);
            query.bindValue(0, item(row, 0)->text());

            if(query.exec()) {
                removeRow(row);
                row -= i;
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
}

void DocumentView::backward()
{
    const QList<QTableWidgetSelectionRange> range = selectedRanges();

    int row = range.isEmpty() ? 1 : range.first().topRow();
    int prev = row - 1;
    selectRow(prev);
}

void DocumentView::toward()
{
    const QList<QTableWidgetSelectionRange> range = selectedRanges();

    int row = range.isEmpty() ? -1 : range.last().bottomRow();
    int next = row + 1;
    selectRow(next);
}

void DocumentView::nextPage()
{
    m_from += m_count;
    refresh();
}

void DocumentView::previousPage()
{
    m_from -= m_count;
    refresh();
}

void DocumentView::gotoPage(int page)
{
   m_from = m_count * (page - 1);
   qDebug() << m_from;
   refresh();
}

void DocumentView::perPage(int count)
{
    m_from = 0;
    m_count = count;
    refresh();
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

