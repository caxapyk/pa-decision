#include "decisionview.h"
#include "ui_decisionview.h"

#include "application.h"
#include "dialogs/decisiondetailsdialog.h"
#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QSqlQuery>

DecisionView::DecisionView(QWidget *parent) :
    View(parent),
    ui(new Ui::DecisionView)
{
    ui->setupUi(this);
    setupShortcuts();

    m_paginator = new Paginator;
    ui->vL_paginator->addWidget(m_paginator);

    initialize();
    restoreViewState();
}

DecisionView::~DecisionView()
{
    saveViewState();
    disconnect(m_proxyModel->sourceModel(), &QAbstractItemModel::modelReset, this, &DecisionView::updated);

    delete ui;
    delete m_paginator;
    delete m_model;
    delete m_proxyModel;

    delete insertShortcut;
    delete editShortcut;
    delete removeShortcut;
    delete refreshShortcut;
}

void DecisionView::initialize()
{
    m_model = new DecisionModel;

    m_proxyModel = new DecisionProxyModel;
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->sort(-1);

    ui->tV_decision->setModel(m_proxyModel);
    ui->tV_decision->sortByColumn(-1, Qt::AscendingOrder);
    ui->tV_decision->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_proxyModel->sourceModel(), &QAbstractItemModel::modelReset, this, &DecisionView::updated);

    connect(ui->tV_decision, &QMenu::customContextMenuRequested, this, &DecisionView::contextMenu);
    connect(ui->tV_decision->selectionModel(), &QItemSelectionModel::selectionChanged, this, &DecisionView::selected);
}

void DecisionView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();

    ui->splitter_data->restoreState(settings->value("Views/splitter_data").toByteArray());
}

void DecisionView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("splitter_data", ui->splitter_data->saveState());
    settings->endGroup();
}

void DecisionView::contextMenu(const QPoint &)
{
    CustomContextMenu menu(CustomContextMenu::All);

    QAction *insertAction = menu.action(CustomContextMenu::Insert);
    insertAction->setShortcut(insertShortcut->key());
    insertAction->setEnabled(true);
    connect(insertAction, &QAction::triggered, this, &DecisionView::insert);

    QAction *editAction = menu.action(CustomContextMenu::Edit);
    editAction->setShortcut(editShortcut->key());
    editAction->setEnabled(editShortcut->isEnabled());
    connect(editAction, &QAction::triggered, this,  &DecisionView::edit);

    QAction *removeAction = menu.action(CustomContextMenu::Remove);
    removeAction->setShortcut(removeShortcut->key());
    removeAction->setEnabled(removeShortcut->isEnabled());
    connect(removeAction, &QAction::triggered, this,  &DecisionView::remove);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(refreshShortcut->key());
    refreshShortcut->setEnabled(true);
    connect(refreshAction, &QAction::triggered, this, &DecisionView::refresh);

    menu.exec(QCursor().pos());
}

void DecisionView::setupShortcuts()
{
    insertShortcut = new QShortcut(QKeySequence::New, ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    insertShortcut->setEnabled(true);
    connect(insertShortcut, &QShortcut::activated, this, &DecisionView::insert);

    editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    editShortcut->setEnabled(false);
    connect(editShortcut, &QShortcut::activated, this, &DecisionView::edit);

    removeShortcut = new QShortcut(QKeySequence::Delete, ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    removeShortcut->setEnabled(false);
    connect(removeShortcut, &QShortcut::activated, this, &DecisionView::remove);

    refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_decision, nullptr, nullptr, Qt::WidgetShortcut);
    setEnabled(true);
    connect(refreshShortcut, &QShortcut::activated, this, &DecisionView::refresh);
}

void DecisionView::edit()
{
    QVariant id = ui->tV_decision->currentIndex().siblingAtColumn(0).data();
    qDebug() << "id passed: " << id;

    DecisionDetailsDialog dialog(id);
    //dialog.setModel(m_model);
    //dialog.setCurrentIndex(m_proxyModel->mapToSource(ui->tV_decision->currentIndex()));
    int res = dialog.exec();

    if(res == DecisionDetailsDialog::Accepted) {
        refresh();
    }
}

void DecisionView::insert()
{
    DecisionDetailsDialog dialog;
    //dialog.setModel(m_model);
    int res = dialog.exec();

    if(res == DecisionDetailsDialog::Accepted) {
        refresh();
    }
}

void DecisionView::refresh()
{
    ui->tV_decision->selectionModel()->clearSelection();

    m_proxyModel->invalidate();
    ui->tV_decision->sortByColumn(-1, Qt::AscendingOrder);
    m_model->select();
}

void DecisionView::remove()
{
    QModelIndexList selected = ui->tV_decision->selectionModel()->selectedRows();

    int res = QMessageBox::critical(this,
                                    tr("Deleting items"),
                                    tr("Are you shure that you want to delete %1 item(s)?").arg(selected.size()),
                                    QMessageBox::No | QMessageBox::Yes);

    if (res == QMessageBox::Yes) {
        for(int i = 0; i < selected.count(); ++i) {
            if(!m_model->primeDelete(selected.at(i).siblingAtColumn(0).data().toInt())) {
                QMessageBox::warning(this,
                                     tr("Deleting item"),
                                     tr("Could not remove the item."),
                                     QMessageBox::Ok);
                break;
            }
        }
    }

    refresh();
}

void DecisionView::selected(const QItemSelection &, const QItemSelection &)
{
    int len = ui->tV_decision->selectionModel()->selectedRows().length();

    editShortcut->setEnabled(len == 1);
    removeShortcut->setEnabled(len > 0);

    application->mainWindow()->action_edit->setEnabled(len == 1);
    application->mainWindow()->action_remove->setEnabled(len > 0);
}

void DecisionView::updated()
{
    ui->lcdn_counter->display(m_model->total());
}
