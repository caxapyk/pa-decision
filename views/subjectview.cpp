#include "subjectview.h"
#include "subjectdelegate.h"
#include "application.h"

#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

SubjectView::SubjectView(QWidget *parent) : TableWidgetView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked);

    setItemDelegate(new SubjectDelegate);

    setColumnCount(m_subjectHeaderLabels.length());
    setHorizontalHeaderLabels(m_subjectHeaderLabels);

    restoreViewState();
}

SubjectView::~SubjectView()
{
    saveViewState();
}

void SubjectView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    horizontalHeader()->restoreState(settings->value("Views/SubjectView").toByteArray());
    horizontalHeader()->setSectionResizeMode(m_subjectHeaderLabels.length() - 1, QHeaderView::Stretch);
    horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
}

void SubjectView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("SubjectView", horizontalHeader()->saveState());
    settings->endGroup();
}

void SubjectView::refresh()
{
    _clear();

    if(m_docId.isValid()) {
        disconnect(this, &SubjectView::itemChanged, nullptr, nullptr);

        QSqlQuery query;

        query.prepare("select pad_subjects.id, pad_subjtype.name, pad_subjects.name, pad_subjects.addresses, pad_subjects.descr, pad_subjects.pages, pad_subjects.subjtype_id from pad_subjects left join pad_subjtype on pad_subjtype.id=subjtype_id where pad_subjects.decision_id=?");
        query.bindValue(0, m_docId);
        query.exec();

        setSortingEnabled(false);

        while(query.next()) {
            int row = rowCount();
            insertRow(row);

            QTableWidgetItem *action_item = new QTableWidgetItem;
            action_item->setFlags(Qt::NoItemFlags);
            setItem(row, 0, action_item);

            for(int j = 1; j < columnCount(); ++j) {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setData(Qt::DisplayRole, query.value(j - 1));
                setItem(row, j, item);
            }

            item(row, 2)->setData(Qt::UserRole, query.value("pad_subjects.subjtype_id"));
        }

        setSortingEnabled(true);

        hideColumn(0);
        hideColumn(1);

        connect(this, &SubjectView::itemChanged, this, &SubjectView::_itemChanged);
    }
}

void SubjectView::contextMenu(BaseContextMenu &menu)
{
    menu.removeAction(menu.action(BaseContextMenu::Edit));
    menu.removeAction(menu.action(BaseContextMenu::Refresh));
}

bool SubjectView::save()
{
    for (int i = 0; i < rowCount(); ++i) {
        QSqlQuery query;

        QString action = item(i, 0)->text();
        if(!action.isEmpty()) {
            if(action == "I" || action =="U") {
                if(item(i, 0)->text() == "I") {
                    query.prepare("insert into pad_subjects(decision_id, subjtype_id, name, addresses, descr, pages) values (:decision_id, :subjtype_id, :name, :addresses, :descr, :pages)");
                    query.bindValue(":decision_id", m_docId);
                } else if(item(i, 0)->text() == "U") {
                    query.prepare("update pad_subjects set subjtype_id=:subjtype_id, name=:name, addresses=:addresses, descr=:descr, pages=:pages where id=:id");
                    query.bindValue(":id", item(i, 1)->data(Qt::DisplayRole));
                }
                query.bindValue(":subjtype_id", item(i, 2)->data(Qt::UserRole));
                query.bindValue(":name", (item(i, 3)->data(Qt::DisplayRole)));
                query.bindValue(":addresses", (item(i, 4)->data(Qt::DisplayRole)));
                query.bindValue(":descr", (item(i, 5)->data(Qt::DisplayRole)));
                query.bindValue(":pages", (item(i, 6)->data(Qt::DisplayRole)));
            } else if(item(i, 0)->text() == "R") {
                query.prepare("delete from pad_subjects where id=:id");
                query.bindValue(":id", item(i, 1)->data(Qt::DisplayRole));
            }

            query.exec();

            if(query.isActive()) {
                if(action == "I") {
                    item(i, 0)->setText(QString());
                    item(i, 1)->setText(query.lastInsertId().toString());
                } else if(action == "U") {
                    item(i, 0)->setText(QString());
                } else if(action == "R") {
                    removeRow(i);
                    --i;
                }
            } else {
                qDebug() << query.lastError().text();
                return false;
            }
        }
    }

    return true;
}

bool SubjectView::validate()
{
    for (int i = 0; i < rowCount(); ++i) {
        if(item(i, 2)->data(Qt::DisplayRole).isNull()) {
            QMessageBox::warning(this,
                    tr("Subjects"),
                    tr("Subjects could not be saved. Fill all subjects required fields."),
                    QMessageBox::Ok);

            return false;
        }
    }

    return true;
}

void SubjectView::_insertRow()
{
    setSortingEnabled(false);

    int row = rowCount();
    insertRow(row);

    QTableWidgetItem *action_item = new QTableWidgetItem("I");
    action_item->setFlags(Qt::NoItemFlags);
    setItem(row, 0, action_item);

    for(int j = 1; j < columnCount(); ++j) {
        QTableWidgetItem *item = new QTableWidgetItem();
        setItem(row, j, item);
    }

    selectRow(row);
    editItem(item(row, 2));

    setSortingEnabled(true);
}

void SubjectView::removeRows()
{
    QList<int> rows = selectedRows();

    int res = QMessageBox::critical(this,
        tr("Documents"),
        tr("Are you shure that you want to delete %1 item(s)?").arg(rows.count()),
        QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        for (int i = 0; i < rows.count(); ++i) {
            int row = rows.at(i);
            QString action = item(row, 0)->text();

            if(action == "I") {
                removeRow(row);
                row -= i;
            } else {
                item(row, 0)->setText("R");
                hideRow(row);
            }

            clearSelection();
        }
    }
}

void SubjectView::_itemChanged(QTableWidgetItem *i)
{
    if(i->column() != 0 && i->column() != 1 && item(i->row(), 0)->text() != "I")
        item(i->row(), 0)->setText("U");
}
