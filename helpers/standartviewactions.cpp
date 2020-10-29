#include "standartviewactions.h"

StandartViewActions::StandartViewActions(QObject *parent) : QObject(parent)
{

}

StandartViewActions::~StandartViewActions()
{
    delete m_insertShortcut;
    delete m_editShortcut;
    delete m_removeShortcut;
    delete m_refreshShortcut;
}

void StandartViewActions::bindShortcuts()
{
    m_insertShortcut = new QShortcut(QKeySequence::New, m_view, nullptr, nullptr, Qt::WidgetShortcut);
    m_editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), m_view, nullptr, nullptr, Qt::WidgetShortcut);
    m_removeShortcut = new QShortcut(QKeySequence::Delete, m_view, nullptr, nullptr, Qt::WidgetShortcut);
    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, m_view, nullptr, nullptr, Qt::WidgetShortcut);

    connect(m_insertShortcut, &QShortcut::activated, m_dialog, &ReferenceDialogInterface::insert);
    connect(m_editShortcut, &QShortcut::activated, m_dialog, &ReferenceDialogInterface::edit);
    connect(m_removeShortcut, &QShortcut::activated, m_dialog, &ReferenceDialogInterface::remove);
    connect(m_refreshShortcut, &QShortcut::activated, m_dialog, &ReferenceDialogInterface::refresh);
}

/*void StandartViewActions::contextMenu(const QPoint &)
{
    QModelIndex currentIndex = ui->tV_record->indexAt(ui->tV_record->viewport()->mapFromGlobal(QCursor().pos()));
    ui->tV_record->setCurrentIndex(currentIndex);

    QMenu menu;
    QString item;

    if(!currentIndex.isValid()) {
        item = tr("fund");
    } else {
        RecordTreeModel::RecordNode *node = static_cast<RecordTreeModel::RecordNode*>(m_proxyModel->mapToSource(currentIndex).internalPointer());
        switch (node->level) {
        case RecordTreeModel::FundLevel:
            item = tr("inventory");
            break;
        case RecordTreeModel::InventoryLevel:
            item = tr("record");
            break;
        }
    }

    QAction insertAction(QIcon(":/icons/icons/add-16.png"), tr("New %1").arg(item));
    insertAction.setShortcut(insertShortcut->key());
    insertAction.setEnabled(insertShortcut->isEnabled());
    connect(&insertAction, &QAction::triggered, this, &RecordDialog::insert);
    menu.addAction(&insertAction);

    QAction editAction(QIcon(":/icons/icons/edit-16.png"), tr("Edit"));
    editAction.setShortcut(editShortcut->key());
    editAction.setEnabled(editShortcut->isEnabled());
    connect(&editAction, &QAction::triggered, this,  &RecordDialog::edit);
    menu.addAction(&editAction);

    QAction removeAction(QIcon(":/icons/icons/remove-16.png"), tr("Remove"));
    removeAction.setShortcut(removeShortcut->key());
    removeAction.setEnabled(removeShortcut->isEnabled());
    connect(&removeAction, &QAction::triggered, this,  &RecordDialog::remove);
    menu.addAction(&removeAction);

    menu.addSeparator();

    QAction refreshAction(QIcon(":/icons/icons/refresh-16.png"), tr("Refresh"));
    refreshAction.setShortcut(refreshShortcut->key());
    connect(&refreshAction, &QAction::triggered, this, &RecordDialog::refresh);

    menu.addAction(&refreshAction);

    menu.exec(QCursor().pos());
}*/
