#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "dialogs/authoritydialog.h"
#include "dialogs/doctypedialog.h"
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

    delete action_doctype;
    delete action_edit;
    delete action_new;
    delete action_pa;
    delete action_print;
    delete action_record;
    delete action_remove;
    //delete action_refresh;
    delete action_tree;

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

    //restore main window geometry and states
    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());

    ui->splitter_layout->restoreState(settings->value("MainWindow/splitter_layout").toByteArray());
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
    action_new = new QAction(QIcon(":/icons/icons/new-24.png"), tr("New"));
    connect(action_new, &QAction::triggered, m_decisionView, &DecisionView::insert);

    action_print = new QAction(QIcon(":/icons/icons/print-24.png"), tr("Print"));
    action_print->setDisabled(true);

    action_edit = new QAction(QIcon(":/icons/icons/edit-24.png"), tr("Edit"));
    action_edit->setDisabled(true);
    connect(action_edit, &QAction::triggered, m_decisionView, &DecisionView::edit);

    action_remove = new QAction(QIcon(":/icons/icons/remove-24.png"), tr("Remove"));
    action_remove->setDisabled(true);

    //action_refresh = new QAction(QIcon(":/icons/icons/refresh-24.png"), tr("Refresh"));

    action_tree = new QAction(QIcon(":/icons/icons/tree-24.png"), tr("Collections"));
    action_tree->setCheckable(true);
    action_tree->setChecked(true);
    connect(action_tree, &QAction::triggered, this, [=]{
         ui->splitter_layout->widget(0)->setHidden(!action_tree->isChecked());
    });

    action_pa = new QAction(QIcon(":/icons/icons/icon-24.png"), tr("Public authorities"));
    connect(action_pa, &QAction::triggered, this, &MainWindow::openAuthorities);

    action_record = new QAction(QIcon(":/icons/icons/record-24.png"), tr("Records"));
    connect(action_record, &QAction::triggered, this, &MainWindow::openRecords);

    action_doctype = new QAction(QIcon(":/icons/icons/doctype-24.png"), tr("Document types"));
    connect(action_doctype, &QAction::triggered, this, &MainWindow::openDoctype);

    ui->toolBar->addAction(action_new);
    ui->toolBar->addAction(action_edit);
    ui->toolBar->addAction(action_remove);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(action_print);
    //ui->toolBar->addAction(action_refresh);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(action_pa);
    ui->toolBar->addAction(action_record);
    ui->toolBar->addAction(action_doctype);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(action_tree);

    m_searchPanel = new SearchPanel();
    ui->toolBar->addWidget(m_searchPanel);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings* settings = application->applicationSettings();

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

void MainWindow::openDoctype()
{
    DoctypeDialog dialog;
    dialog.exec();
}

void MainWindow::openConnection()
{
    ConnectionDialog dialog;
    dialog.exec();
}

void MainWindow::openAuthorities()
{
    AuthorityDialog dialog;
    int res = dialog.exec();

    if(res == AuthorityDialog::Accepted && m_navigatorView->currentCollection() == NavigatorView::CollectionAuthority) {
        m_navigatorView->refresh();
    }
}

void MainWindow::openRecords()
{
    RecordDialog dialog;
    int res = dialog.exec();

    if(res == AuthorityDialog::Accepted && m_navigatorView->currentCollection() == NavigatorView::CollectionRecord) {
        m_navigatorView->refresh();
    }
}
