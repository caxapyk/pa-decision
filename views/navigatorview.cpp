#include "navigatorview.h"
#include "ui_navigatorview.h"

#include "application.h"
#include "models/recordmodel.h"
#include "models/recordproxymodel.h"
#include "models/documenttypemodel.h"
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
    delete m_collectionModel;
    delete m_collectionProxyModel;
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

    connect(ui->tV_authority, &QTreeView::clicked, this, [=] { loadGroup(currentGroupIndex); });

    ui->tV_collection->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tV_collection, &QMenu::customContextMenuRequested, this, &NavigatorView::contextMenu);

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_collection, nullptr, nullptr, Qt::WidgetShortcut);
    connect(m_refreshShortcut, &QShortcut::activated, this, [=] { loadGroup(currentGroupIndex); });

    connect(ui->cB_collection, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &NavigatorView::loadGroup);
}

void NavigatorView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();

    ui->splitter_nav->restoreState(settings->value("Views/splitter_nav").toByteArray());
    ui->tV_authority->header()->restoreState(settings->value("Views/tV_authority").toByteArray());

    int currentModel = settings->contains("Views/cB_collection") ? settings->value("Views/cB_collection").toInt() : 0;
    ui->cB_collection->setCurrentIndex(currentModel);
    loadGroup(currentModel);

    ui->tV_collection->header()->restoreState(settings->value("Views/tV_collection").toByteArray());
}

void NavigatorView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("splitter_nav", ui->splitter_nav->saveState());
    settings->setValue("tV_authority", ui->tV_authority->header()->saveState());
    settings->setValue("tV_collection", ui->tV_collection->header()->saveState());
    settings->setValue("cB_collection", QVariant(ui->cB_collection->currentIndex()));
    settings->endGroup();
}

void NavigatorView::setExplorer(ExplorerView *exp)
{
    _explorer = exp;
    openIndexTab();
}

void NavigatorView::contextMenu(const QPoint &)
{
    CustomContextMenu menu(CustomContextMenu::Refresh);

    QAction openInNTab(tr("Open in new tab"));
    connect(&openInNTab, &QAction::triggered, this, [=] {
        QModelIndex currentIndex = ui->tV_authority->indexAt(ui->tV_authority->viewport()->mapFromGlobal(QCursor().pos()));

        if(currentIndex.isValid()) {
            DecisionTab *tab = new DecisionTab;
            tab->model()->select();

            explorer()->createTab(new DecisionTab, currentIndex.data().toString());
        }
    });
    menu.insertAction(menu.action(CustomContextMenu::Refresh), &openInNTab);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(m_refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, [=] {
        loadGroup(ui->cB_collection->currentIndex());
    });

    menu.exec(QCursor().pos());
}

void NavigatorView::openIndexTab() {
    // index tab page
}

void NavigatorView::refreshAuthority()
{
    m_authorityModel->select();
    ui->tV_authority->expandAll();
}

void NavigatorView::refreshGroup()
{
    loadGroup(currentGroupIndex);
}

void NavigatorView::loadGroup(int collection)
{
    delete m_collectionModel;
    delete m_collectionProxyModel;

    switch(collection){
    case NavigatorView::GroupRecord:
        m_collectionModel = new RecordModel();
        m_collectionProxyModel = new RecordProxyModel;
        currentGroupIndex = NavigatorView::GroupRecord;
        break;
    case NavigatorView::GroupProtocol:
        currentGroupIndex = NavigatorView::GroupProtocol;
        break;
    case NavigatorView::GroupDoctype:
        m_collectionModel = new DocumentTypeModel;
        m_collectionProxyModel = new QSortFilterProxyModel;
        currentGroupIndex = NavigatorView::GroupDoctype;
        break;
    case NavigatorView::GroupYear:
        currentGroupIndex = NavigatorView::GroupYear;
        break;
    case NavigatorView::GroupSubject:
        currentGroupIndex = NavigatorView::GroupSubject;
        break;
    }

    if(m_collectionModel == nullptr)
        return;

    QModelIndex v = ui->tV_authority->currentIndex();
    if(v.parent().isValid()) {
        int id = v.data(Qt::UserRole).toInt();
        m_collectionModel->setAuthorityId(id);
    }

    m_collectionModel->select();

    m_collectionProxyModel->setSourceModel(m_collectionModel);
    ui->tV_collection->setModel(m_collectionProxyModel);

    for(int i = 1; i < ui->tV_collection->model()->columnCount(); ++i)
        ui->tV_collection->hideColumn(i);
}

