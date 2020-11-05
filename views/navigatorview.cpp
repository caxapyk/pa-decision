#include "navigatorview.h"
#include "ui_navigatorview.h"

#include "application.h"
#include "models/authoritymodel.h"
#include "models/authorityproxymodel.h"
#include "models/recordmodel.h"
#include "models/recordproxymodel.h"
#include "widgets/customcontextmenu.h"

#include <QDebug>
#include <QMenu>

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
    delete m_model;
    delete m_proxyModel;
}

void NavigatorView::initialize()
{
    ui->tV_collection->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tV_collection, &QMenu::customContextMenuRequested, this, &NavigatorView::contextMenu);

    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, ui->tV_collection, nullptr, nullptr, Qt::WidgetShortcut);
    connect(m_refreshShortcut, &QShortcut::activated, this, [=] {
        switchModel(ui->cB_collection->currentIndex());
    });

    connect(ui->cB_collection, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &NavigatorView::switchModel);
}

void NavigatorView::restoreViewState()
{
    QSettings* settings = application->applicationSettings();

    int currentModel = settings->contains("Views/cB_collection") ? settings->value("Views/cB_collection").toInt() : 0;
    ui->cB_collection->setCurrentIndex(currentModel);
    switchModel(currentModel);

    ui->tV_collection->header()->restoreState(settings->value("Views/tV_collection").toByteArray());
}

void NavigatorView::saveViewState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("Views");
    settings->setValue("tV_collection", ui->tV_collection->header()->saveState());
    settings->setValue("cB_collection", QVariant(ui->cB_collection->currentIndex()));
    settings->endGroup();
}

void NavigatorView::contextMenu(const QPoint &)
{
    CustomContextMenu menu(CustomContextMenu::Refresh);

    QAction *refreshAction = menu.action(CustomContextMenu::Refresh);
    refreshAction->setShortcut(m_refreshShortcut->key());
    connect(refreshAction, &QAction::triggered, this, [=] {
        switchModel(ui->cB_collection->currentIndex());
    });

    menu.exec(QCursor().pos());
}

void NavigatorView::refresh()
{
    switchModel(current);
}

void NavigatorView::switchModel(int index)
{
    delete m_model;
    delete m_proxyModel;

    switch(index){
    case NavigatorView::CollectionAuthority:
        m_model = new AuthorityModel();
        m_proxyModel = new AuthorityProxyModel;

        current = NavigatorView::CollectionAuthority;
        break;
    case NavigatorView::CollectionRecord:
        m_model = new RecordModel();
        m_proxyModel = new RecordProxyModel;

        current = NavigatorView::CollectionRecord;
        break;
    case NavigatorView::CollectionProtocol:
        m_model = new AuthorityModel();
        m_proxyModel = new AuthorityProxyModel;

        current = NavigatorView::CollectionAuthority;
        break;
    case NavigatorView::CollectionYear:
        break;
    case NavigatorView::CollectionObject:
        break;
    }

    m_model->select();

    m_proxyModel->setSourceModel(m_model);
    ui->tV_collection->setModel(m_proxyModel);

    if(current == CollectionAuthority) {
        ui->tV_collection->expandAll();
    }
}

