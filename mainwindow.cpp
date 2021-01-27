#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "dialogs/doctypedialog.h"
#include "dialogs/connectiondialog.h"
#include "dialogs/protocoldialog.h"
#include "dialogs/recorddialog.h"
#include "dialogs/subjtypedialog.h"

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
    delete m_authorityTreeView;
//    /delete m_explorerView;
    delete m_statusBarPanel;
    //delete m_searchPanel;

    delete action_edit;
    delete action_new;
    delete action_print;
    delete action_remove;
    delete action_refresh;
    delete action_tree;
    delete action_record;

    //delete m_searchShortcut;
}

void MainWindow::initialize()
{
    m_authorityTreeView = new AuthorityTreeView;
    ui->tab_authorities->layout()->addWidget(m_authorityTreeView);
    //m_explorerView = new ExplorerView(ui->splitter_layout);
    //m_authorityTreeView->setExplorer(m_explorerView);

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

void MainWindow::saveAppState()
{
    QSettings* settings = application->applicationSettings();

    settings->beginGroup("MainWindow");
    settings->setValue("geometry", saveGeometry());
    settings->setValue("windowState", saveState());
    settings->setValue("splitter_layout", ui->splitter_layout->saveState());
    settings->endGroup();
}

void MainWindow::setupShortcuts()
{
    //m_searchShortcut = new QShortcut(QKeySequence::Find, this);
    /*connect(m_searchShortcut, &QShortcut::activated, this, [=] {
        m_searchPanel->setFocus();
    });*/
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
    action_new->setDisabled(true);
    connect(action_new, &QAction::triggered, this, [=] {
        //m_authorityTreeView->currentDecisionView()->insert();
    });

    action_print = new QAction(QIcon(":/icons/icons/print-24.png"), tr("Print"));
    action_print->setDisabled(true);

    action_edit = new QAction(QIcon(":/icons/icons/edit-24.png"), tr("Edit"));
    action_edit->setDisabled(true);
    connect(action_edit, &QAction::triggered, this, [=] {
        //m_authorityTreeView->currentDecisionView()->edit();
    });

    action_remove = new QAction(QIcon(":/icons/icons/remove-24.png"), tr("Remove"));
    action_remove->setDisabled(true);
    connect(action_remove, &QAction::triggered, this, [=] {
       // m_authorityTreeView->currentDecisionView()->remove();
    });

    action_refresh = new QAction(QIcon(":/icons/icons/refresh-24.png"), tr("Refresh"));
    action_refresh->setDisabled(true);
    connect(action_refresh, &QAction::triggered, this, [=] {
        //m_authorityTreeView->currentDecisionView()->refresh();
    });

    action_tree = new QAction(QIcon(":/icons/icons/tree-24.png"), tr("Left panel"));
    action_tree->setCheckable(true);
    action_tree->setChecked(true);
    connect(action_tree, &QAction::triggered, this, [=]{
         ui->splitter_layout->widget(0)->setHidden(!action_tree->isChecked());
    });

    action_record = new QAction(QIcon(":/icons/icons/record-24.png"), tr("Records"));
    action_record->setDisabled(true);
    connect(action_record, &QAction::triggered, this, [=] {
        RecordDialog *dialog = new RecordDialog(m_authorityTreeView->id());
        openDialog(dialog);
    });

    ui->toolBar->addAction(action_new);
    ui->toolBar->addAction(action_edit);
    ui->toolBar->addAction(action_remove);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(action_print);
    ui->toolBar->addAction(action_refresh);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(action_tree);
    ui->toolBar->addSeparator();

    ui->toolBar->addAction(action_record);

    m_referenceButton = new ReferenceButton;
    connect(m_referenceButton->actionDoctype(), &QAction::triggered, this, [=] { openDialog(new DoctypeDialog); });
    connect(m_referenceButton->actionSubjtype(), &QAction::triggered, this, [=] { openDialog(new SubjtypeDialog); });
    ui->toolBar->addWidget(m_referenceButton);

    //m_searchPanel = new SearchPanel;
    //ui->toolBar->addWidget(m_searchPanel);
}

/*void Authority0View::openInNewTab(const QModelIndex &index)
{
    if(index.isValid() && index.parent().isValid()) {
        DecisionTab *tab = new DecisionTab(index.data(Qt::UserRole));
        QIcon icon(index.data(Qt::DecorationRole).toString());

        explorer()->createTab(tab, index.data().toString(), icon);
    }
}*/

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
