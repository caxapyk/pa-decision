#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "dialogs/connectiondialog.h"
#include "dialogs/recorddialog.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize();

    // MainWindow actions
    connect(ui->action_about, &QAction::triggered, application, &Application::about);
    connect(ui->action_connection, &QAction::triggered, this, &MainWindow::openConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_navigatorView;
    delete m_decisionView;
    delete m_statusBarPanel;
    delete m_searchPanel;

    delete action_new;
    delete action_edit;
    delete action_print;
    delete action_record;
    delete action_remove;
    delete action_refresh;
    delete action_tree;

    delete m_editShortcut;
    delete m_newShortcut;
    delete m_refreshShortcut;
    delete m_removeShortcut;
    delete m_searchShortcut;
}

void MainWindow::initialize()
{
    m_navigatorView = new NavigatorView(ui->splitter_layout);
    m_decisionView = new DecisionView(ui->splitter_layout);

    ui->splitter_layout->setCollapsible(0, false);

    setupShortcuts();
    setupToolBar();
    setupStatusBar();
    restoreAppState();
}

void MainWindow::restoreAppState()
{
    QSettings* settings = application->applicationSettings();

    m_navigatorView->restoreViewState();

    //restore main window geometry and states
    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());

    ui->splitter_layout->restoreState(settings->value("MainWindow/splitter_layout").toByteArray());
}

void MainWindow::setupShortcuts()
{
    m_editShortcut = new QShortcut(QKeySequence(Qt::Key_F2), this);
    m_newShortcut = new QShortcut(QKeySequence::New, this);
    m_refreshShortcut = new QShortcut(QKeySequence::Refresh, this);
    m_removeShortcut = new QShortcut(QKeySequence::Delete, this);

    m_searchShortcut = new QShortcut(QKeySequence::Find, this);
    connect(m_searchShortcut, &QShortcut::activated, this, [=] {
        m_searchPanel->setFocus();
    });
}

void MainWindow::setupStatusBar()
{
    m_statusBarPanel = new StatusBarPanel;
    m_statusBarPanel->setTotal(0);
    ui->statusbar->addPermanentWidget(m_statusBarPanel);

    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::setupToolBar()
{
    action_new = new QAction(QIcon(":/icons/icons/new-24.png"), tr("New"));
    action_print = new QAction(QIcon(":/icons/icons/print-24.png"), tr("Print"));
    action_edit = new QAction(QIcon(":/icons/icons/edit-24.png"), tr("Edit"));

    action_record = new QAction(QIcon(":/icons/icons/record-24.png"), tr("Records"));
    connect(action_record, &QAction::triggered, this, &MainWindow::openRecords);


    action_remove = new QAction(QIcon(":/icons/icons/remove-24.png"), tr("Remove"));
    action_refresh = new QAction(QIcon(":/icons/icons/refresh-24.png"), tr("Refresh"));

    action_tree = new QAction(QIcon(":/icons/icons/tree-24.png"), tr("Tree"));
    action_tree->setCheckable(true);
    action_tree->setChecked(true);
    connect(action_tree, &QAction::triggered, this, [=]{
         ui->splitter_layout->widget(0)->setHidden(!action_tree->isChecked());
    });

    ui->toolBar->addAction(action_new);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(action_edit);
    ui->toolBar->addAction(action_remove);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(action_print);
    ui->toolBar->addAction(action_refresh);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(action_tree);
    ui->toolBar->addAction(action_record);

    m_searchPanel = new SearchPanel();
    ui->toolBar->addWidget(m_searchPanel);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings* settings = application->applicationSettings();

    m_navigatorView->saveViewState();

    settings->beginGroup("MainWindow");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());
    settings->setValue("splitter_layout", ui->splitter_layout->saveState());
    settings->endGroup();

    QMainWindow::closeEvent(event);
}

/* *
 * Dialogs
 * */

void MainWindow::openConnection()
{
    ConnectionDialog dialog;
    dialog.exec();
}

void MainWindow::openRecords()
{
    RecordDialog dialog;
    dialog.exec();
}
