#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "dialogs/connectiondialog.h"

#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize();
    restoreAppState();

    // MainWindow actions
    connect(ui->action_about, &QAction::triggered, application, &Application::about);
    connect(ui->action_connection, &QAction::triggered, this, &MainWindow::openConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_authorityView;
    delete m_decisionView;
}

void MainWindow::initialize()
{
    m_authorityView = new AuthorityView(ui->splitter_layout);
    m_decisionView = new DecisionView(ui->splitter_layout);
}

void MainWindow::restoreAppState()
{
    QSettings* settings = application->applicationSettings();

    //restore main window geometry and states
    restoreGeometry(settings->value("MainWindow/geometry").toByteArray());
    restoreState(settings->value("MainWindow/windowState").toByteArray());

    ui->splitter_layout->restoreState(settings->value("MainWindow/splitter_layout").toByteArray());
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

void MainWindow::openConnection()
{
    ConnectionDialog dialog;
    dialog.exec();
}
