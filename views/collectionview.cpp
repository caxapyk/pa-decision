#include "collectionview.h"
#include "ui_collectionview.h"

#include "application.h"

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
    QMenu menu;

    QAction refreshAction(QIcon(":/icons/icons/refresh-16.png"), tr("Refresh"));
    refreshAction.setShortcut(m_refreshShortcut->key());
    connect(&refreshAction, &QAction::triggered, this, [=] {
        switchModel(ui->cB_collection->currentIndex());
    });

    menu.addAction(&refreshAction);

    menu.exec(QCursor().pos());
}

void CollectionView::switchModel(int index)
{
    switch(index){
    case CollectionView::CollectionRecord:
        setRecordCollection();
        break;
    case CollectionView::CollectionYear:
        break;
    case CollectionView::CollectionMember:
        break;
    }
}

void CollectionView::setRecordCollection()
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
