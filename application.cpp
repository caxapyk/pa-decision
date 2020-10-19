#include "application.h"
#include "connection.h"
#include "mainwindow.h"

#include "dialogs/connectiondialog.h"

#include <QDebug>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QIcon>
#include <QLibraryInfo>
#include <QMessageBox>
#include <QStringList>
#include <QTimer>

Application* application = nullptr;

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    application = this; //global pointer to current instance

    initializeSettings();
    initializeLanguage();

    QTimer::singleShot(0, this, &Application::connect);
}

Application::~Application()
{
    delete m_mainWindow;
    delete m_settings;
    delete qtTranslator;
    delete appTranslator;
}

void Application::initMainWindow()
{
    m_mainWindow = new MainWindow;
    m_mainWindow->show();
}

void Application::connect()
{
    QCommandLineParser parser;
    parser.setApplicationDescription(tr("Public Authority's Decisions Database"));
    parser.addVersionOption();

    QCommandLineOption hostnameOption(QStringList() << "h" << "hostname", tr("Address of database server"), "localhost");
    parser.addOption(hostnameOption);
    QCommandLineOption databaseOption(QStringList() << "d" << "database", tr("Database name"), "tpointer");
    parser.addOption(databaseOption);
    QCommandLineOption usernameOption(QStringList() << "u" << "username", tr("Database user"), "root");
    parser.addOption(usernameOption);
    QCommandLineOption passwordOption(QStringList() << "p" << "password", tr("Password to database user"), "pass");
    parser.addOption(passwordOption);

    parser.process(*application);

    server_address = parser.value(hostnameOption);

    try {
        Connection conn;

        m_settings->beginGroup("Database");
        QString hostname = !parser.value(hostnameOption).isEmpty() ? parser.value(hostnameOption) : m_settings->value("hostname").toString();
        QString database = !parser.value(databaseOption).isEmpty() ? parser.value(databaseOption) : m_settings->value("db").toString();
        QString username = !parser.value(usernameOption).isEmpty() ? parser.value(usernameOption) : m_settings->value("user").toString();
        QString password = !parser.value(passwordOption).isEmpty() ? parser.value(passwordOption) : m_settings->value("password").toString();
        m_settings->endGroup();

        conn.connect(hostname, database, username, password);

        initMainWindow();
    } catch (ConnectionExeption e) {
        qCritical() << QString(e.p);
        int res = QMessageBox::critical(nullptr, tr("Database connection"), tr("Could not connect to database.\nDo you want to config now?"), QMessageBox::Ok | QMessageBox::Cancel);

        if (res == QMessageBox::Ok) {
            ConnectionDialog dialog;
            int cres = dialog.exec();
            if (cres == ConnectionDialog::Accepted) {
                connect();
            } else {
                QCoreApplication::exit(EXIT_FAILURE);
            }
        } else {
            QCoreApplication::exit(EXIT_SUCCESS);
        }
    }
}

void Application::initializeSettings()
{
    setOrganizationName(m_orgName);
    setApplicationName(m_appName);
    setApplicationVersion(m_version.toString());

    m_settings = new QSettings(QSettings::NativeFormat,
                               QSettings::UserScope, m_orgName, m_appName);
}

void Application::initializeLanguage()
{
    qtTranslator = new QTranslator(this);
    appTranslator = new QTranslator(this);

    if(qtTranslator->load("qt_" + QLocale::system().name(),
             QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        installTranslator(qtTranslator);
    } else if (qtTranslator->load("qt_ru.qm", ":/translations/")) {
         installTranslator(qtTranslator);
    }

    if (appTranslator->load("pa-decision_ru_RU.qm", ":/translations/")) {
        installTranslator(appTranslator);
    }
}

void Application::about() const {
    QString message;
    message += "<h3>" + tr("Public Authority's Decisions Database") + tr(" v %1").arg(version().toString()) + "</h3>";
    message += "<p>" + tr("This program is free software: you can redistribute it and/or modify"
        " it under the terms of the GNU General Public License as published by"
        " the Free Software Foundation, either version 3 of the License, or"
    " (at your option) any later version.") + "</p>";

    message += "<p><span>" + tr("Application icons by ") + "<a href=https://icons8.com>" + "Icons8" + "</a>" + ".</span></p>";

    message += "<p>" + tr("Copyright &copy; 2020 Alexander Sakharuk") + "</p>";

    QMessageBox aboutBox;
    aboutBox.setWindowTitle(tr("About"));
    aboutBox.setText(message);
    aboutBox.setWindowIcon(icon());

    aboutBox.exec();
}
