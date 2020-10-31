#include "collectionview.h"
#include "ui_collectionview.h"

#include "application.h"
#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QKeyEvent>
#include <QMenu>

CollectionView::CollectionView(QWidget *parent) :
    View(parent),
    ui(new Ui::CollectionView)
{
    ui->setupUi(this);

    initialize();
}

CollectionView::~CollectionView()
{
    delete ui;
    delete m_authorityModel;
    delete m_authorityProxyModel;
    delete m_recordModel;
    delete m_recordProxyModel;
    delete m_refreshShortcut;
}

void CollectionView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();
    ui->tV_collection->header()->restoreState(settings->value("Views/tV_collection").toByteArray());
}

void CollectionView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("tV_collection", ui->tV_collection->header()->saveState());
    settings->endGroup();
}

void CollectionView::initialize()
{
    ui->tV_collection->sortByColumn(0, Qt::AscendingOrder);
    ui->tV_collection->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tV_collection, &QMenu::customContextMenuRequested, this, &CollectionView::contextMenu);

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_collection, nullptr, nullptr, Qt::WidgetShortcut);
    connect(m_refreshShortcut, &QShortcut::activated, this, [=] {
        switchModel(ui->cB_collection->currentIndex());
    });

    connect(ui->cB_collection, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &CollectionView::switchModel);

    switchModel(0);
}

void CollectionView::contextMenu(const QPoint &)
{
    CustomContextMenu menu(CustomContextMenu::Refresh);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(m_refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, [=] {
        switchModel(ui->cB_collection->currentIndex());
    });

    menu.exec(QCursor().pos());
}

void CollectionView::switchModel(int index)
{
    switch(index){
    case CollectionView::CollectionAuthority:
        setCollectionAuthority();
        break;
    case CollectionView::CollectionRecord:
        setCollectionRecord();
        break;
    case CollectionView::CollectionYear:
        break;
    case CollectionView::CollectionMember:
        break;
    }
}

void CollectionView::setCollectionAuthority()
{
    if(m_recordModel == nullptr) {
        m_authorityModel = new AuthorityTreeModel;
        m_authorityModel->select();

        m_authorityProxyModel = new AuthorityProxyModel;
        m_authorityProxyModel->setSourceModel(m_authorityModel);
    }

    m_authorityProxyModel->invalidate();
    m_authorityModel->select();

    ui->tV_collection->setModel(m_authorityProxyModel);
    ui->tV_collection->setCurrentIndex(m_authorityProxyModel->mapFromSource(m_authorityModel->rootItem()));
    ui->tV_collection->expandAll();
}

void CollectionView::setCollectionRecord()
{
    if(m_recordModel == nullptr) {
        m_recordModel = new RecordTreeModel;

        m_recordProxyModel = new RecordProxyModel;
        m_recordProxyModel->setSourceModel(m_recordModel);
    }

    m_recordProxyModel->invalidate();
    m_recordModel->select();

    ui->tV_collection->setModel(m_recordProxyModel);
}
