#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "dialogs/afdialog.h"
#include "dialogs/connectiondialog.h"
#include "dialogs/documenttypedialog.h"
#include "dialogs/protocoldialog.h"
#include "dialogs/subjecttypedialog.h"
#include "views/afview.h"
#include "widgets/documenttab.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize();

    // MainWindow actions
    connect(ui->action_about, &QAction::triggered, application, &Application::about);
    connect(ui->action_connection, &QAction::triggered, this, [=] { openDialog(new ConnectionDialog); });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_authorityView;
    delete m_explorer;
    delete m_statusBarPanel;
    delete m_searchPanel;

    delete m_actionEdit;
    delete m_actionInsert;
    delete m_actionPrint;
    delete m_actionRemove;
    delete m_actionRefresh;
    delete m_actionTree;
    delete m_actionAF;

    delete m_searchShortcut;
}

void MainWindow::initialize()
{
    setupShortcuts();
    setupToolBar();
    setupStatusBar();
    restoreAppState();

    m_authorityView = new AuthorityView;
    ui->tab_authorities->layout()->addWidget(m_authorityView);

    connect(m_authorityView, &AuthorityView::openInNewTabRequested, this, &MainWindow::openAuthorityInNewTab);

    m_explorer = new Explorer;

    ui->vL_data->insertWidget(0, m_explorer);
    ui->splitter_layout->setCollapsible(0, false);
}

void MainWindow::restoreAppState()
{
    QSettings* settings = application->applicationSettings();

    //restore main window geometry and states
    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());

    ui->splitter_layout->restoreState(settings->value("MainWindow/splitter_layout").toByteArray());
    ui->splitter_document->restoreState(settings->value("MainWindow/splitter_document").toByteArray());
}

void MainWindow::saveAppState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("MainWindow");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());
    settings->setValue("splitter_layout", ui->splitter_layout->saveState());
    settings->setValue("splitter_document", ui->splitter_document->saveState());
    settings->endGroup();
}

void MainWindow::setupShortcuts()
{
    m_searchShortcut = new QShortcut(QKeySequence::Find, this);
    connect(m_searchShortcut, &QShortcut::activated, this, [=] {
        m_searchPanel->setFocus();
    });
}

void MainWindow::setupStatusBar()
{
    m_statusBarPanel = new StatusBarPanel;
    ui->statusbar->addPermanentWidget(m_statusBarPanel);

    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setupToolBar()
{
    m_actionInsert = new QAction(QIcon(":/icons/icons/new-24.png"), tr("New"));
    m_actionInsert->setDisabled(true);
    connect(m_actionInsert, &QAction::triggered, this, [=] {
        Tab *tab = m_explorer->currentTab();
        DocumentTab *dt = qobject_cast<DocumentTab*>(tab);

        if(dt != nullptr) {
            dt->view()->_insertRow();
        }
    });

    m_actionPrint = new QAction(QIcon(":/icons/icons/print-24.png"), tr("Print"));
    m_actionPrint->setDisabled(true);

    m_actionEdit = new QAction(QIcon(":/icons/icons/edit-24.png"), tr("Edit"));
    m_actionEdit->setDisabled(true);
    connect(m_actionEdit, &QAction::triggered, this, [=] {
        Tab *tab = m_explorer->currentTab();
        DocumentTab *dt = qobject_cast<DocumentTab*>(tab);

        if(dt != nullptr) {
           dt->view()->editRow();
        }
    });

    m_actionRemove = new QAction(QIcon(":/icons/icons/remove-24.png"), tr("Remove"));
    m_actionRemove->setDisabled(true);
    connect(m_actionRemove, &QAction::triggered, this, [=] {
        Tab *tab = m_explorer->currentTab();
        DocumentTab *dt = qobject_cast<DocumentTab*>(tab);

        if(dt != nullptr) {
            dt->view()->removeRows();
        }
    });

    m_actionRefresh = new QAction(QIcon(":/icons/icons/refresh-24.png"), tr("Refresh"));
    m_actionRefresh->setDisabled(true);
    connect(m_actionRefresh, &QAction::triggered, this, [=] {
        Tab *tab = m_explorer->currentTab();
        DocumentTab *dt = qobject_cast<DocumentTab*>(tab);

        if(dt != nullptr) {
            dt->view()->refresh();
        }
    });

    m_actionTree = new QAction(QIcon(":/icons/icons/tree-24.png"), tr("Left panel"));
    m_actionTree->setCheckable(true);
    m_actionTree->setChecked(true);
    connect(m_actionTree, &QAction::triggered, this, [=]{
         ui->splitter_layout->widget(0)->setHidden(!m_actionTree->isChecked());
    });

    m_actionAF = new QAction(QIcon(":/icons/icons/record-24.png"), tr("Records"));
    m_actionAF->setDisabled(true);
    connect(m_actionAF, &QAction::triggered, this, &MainWindow::openAF);

    ui->toolBar->addAction(m_actionInsert);
    ui->toolBar->addAction(m_actionEdit);
    ui->toolBar->addAction(m_actionRemove);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(m_actionPrint);
    ui->toolBar->addAction(m_actionRefresh);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(m_actionTree);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(m_actionAF);

    m_referenceButton = new ReferenceButton;
    connect(m_referenceButton->actionDoctype(), &QAction::triggered, this, [=] { openDialog(new DocumentTypeDialog); });
    connect(m_referenceButton->actionSubjtype(), &QAction::triggered, this, [=] { openDialog(new SubjectTypeDialog); });
    ui->toolBar->addWidget(m_referenceButton);

    ui->toolBar->addSeparator();

    m_searchPanel = new SearchPanel;
    m_searchPanel->setDisabled(true);
    ui->toolBar->addWidget(m_searchPanel);
}

void MainWindow::openAF()
{
    Tab *tab = m_explorer->currentTab();
    DocumentTab *dt = qobject_cast<DocumentTab*>(tab);

    if(dt != nullptr) {
        AFDialog dialog;

        AFView *view = dynamic_cast<AFView*>(dialog.treeView());
        view->model()->setAuthorityId(dt->view()->authorityId());

        dialog.exec();
    }
}

void MainWindow::openAuthorityInNewTab(const QVariant &id)
{
    if(id.isValid()) {
        const QModelIndex v = m_authorityView->currentIndex();
        DocumentTab *tab = new DocumentTab(id);
        QIcon icon(v.data(Qt::DecorationRole).toString());

        m_explorer->createTab(tab, v.data().toString(), icon);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveAppState();
    QMainWindow::closeEvent(event);
}

void MainWindow::openDialog(QDialog *dialog) {
    dialog->exec();

    if(dialog != nullptr)
        delete dialog;
}
