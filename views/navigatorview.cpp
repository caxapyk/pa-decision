#include "navigatorview.h"
#include "ui_navigatorview.h"

#include "application.h"
#include "widgets/customcontextmenu.h"
#include "widgets/decisiontab.h"

#include <QDebug>
#include <QMenu>
#include <QSortFilterProxyModel>

NavigatorView::NavigatorView(QWidget *parent) :
    View(parent),
    ui(new Ui::NavigatorView)
{
    ui->setupUi(this);
    initialize();

    restoreViewState();
}

NavigatorView::~NavigatorView()
{
    saveViewState();

    delete ui;

    delete m_authorityModel;
    delete m_authorityProxyModel;
}

void NavigatorView::initialize()
{
    m_authorityModel = new AuthorityModel;
    m_authorityModel->select();

    m_authorityProxyModel = new AuthorityProxyModel;
    m_authorityProxyModel->setSourceModel(m_authorityModel);
    ui->tV_authority->setModel(m_authorityProxyModel);

    ui->tV_authority->hideColumn(1);
    ui->tV_authority->hideColumn(2);
    ui->tV_authority->expandAll();

    ui->tV_authority->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tV_authority, &QMenu::customContextMenuRequested, this, &NavigatorView::contextMenu);

    connect(ui->tV_authority, &QTreeView::doubleClicked, this, &NavigatorView::openInNewTab);

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_authority, nullptr, nullptr, Qt::WidgetShortcut);
    connect(m_refreshShortcut, &QShortcut::activated, this, [=] { m_authorityModel->select(); });
}

void NavigatorView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    ui->tV_authority->header()->restoreState(settings->value("Views/tV_authority").toByteArray());
}

void NavigatorView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("tV_authority", ui->tV_authority->header()->saveState());
    settings->endGroup();
}

void NavigatorView::setExplorer(ExplorerView *exp)
{
    _explorer = exp;
    openIndexTab();
}

void NavigatorView::contextMenu(const QPoint &)
{
    QModelIndex currentIndex = ui->tV_authority->indexAt(ui->tV_authority->viewport()->mapFromGlobal(QCursor().pos()));

    CustomContextMenu menu(CustomContextMenu::Refresh);

    QAction openInNTAction(tr("Open in new tab"));
    connect(&openInNTAction, &QAction::triggered, this, [=] {
        openInNewTab(currentIndex);
    });
    openInNTAction.setEnabled(currentIndex.isValid());
    menu.insertAction(menu.action(CustomContextMenu::Refresh), &openInNTAction);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(m_refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, [=] {
        m_authorityModel->select();
    });

    menu.exec(QCursor().pos());
}

void NavigatorView::openInNewTab(const QModelIndex &index)
{
    if(index.isValid()) {
        DecisionTab *tab = new DecisionTab;
        tab->view()->model()->setAuthorityId(index.data(Qt::UserRole).toInt());
        tab->view()->model()->select();

        QIcon icon(index.data(Qt::DecorationRole).toString());

        explorer()->createTab(tab, index.data().toString(), icon);
    }
}

void NavigatorView::openIndexTab() {
    // index tab page
}
