#include "collectionview.h"
#include "ui_collectionview.h"

#include "application.h"

#include <QDebug>

CollectionView::CollectionView(QWidget *parent) :
    View(parent),
    ui(new Ui::CollectionView)
{
    ui->setupUi(this);

    switchModel();
}

CollectionView::~CollectionView()
{
    delete ui;
    delete m_recordModel;
    delete m_recordProxyModel;
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


void CollectionView::switchModel()
{
    switch(ui->cB_collection->currentIndex()){
    case CollectionView::CollectionRecord:
        if(m_recordModel == nullptr) {
            m_recordModel = new RecordTreeModel;
            m_recordModel->select();

            m_recordProxyModel = new RecordProxyModel;
            m_recordProxyModel->setSourceModel(m_recordModel);

            ui->tV_collection->setModel(m_recordProxyModel);
        } else {
            ui->tV_collection->setModel(m_recordProxyModel);
        }
        break;
    case CollectionView::CollectionYear:
        break;
    case CollectionView::CollectionMember:
        break;
    }
}
