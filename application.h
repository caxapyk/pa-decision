#ifndef APPLICATION_H
#define APPLICATION_H

#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QTranslator>
#include <QString>
#include <QSettings>

class Application : public QApplication
{

    Q_OBJECT

public:
    Application(int &argc, char **argv);
    ~Application();
    QSettings* applicationSettings() { return m_settings; }
    QVariant version() const { return m_version; };
    QIcon icon() const{ return m_icon; };
    QString basename() { return QString(tr("Records Invetories Register %1").arg(version().toString())); };
    MainWindow *mainWindow() { return m_mainWindow; };
    void connect();

    QString server_address;

    const QString m_orgName = "GAOO";
    const QString m_appName = "RA-Decision";
    const QVariant m_version = QVariant(QString::number(APP_VERSION, 'f', 1));
    const QIcon m_icon = QIcon(":/icons/icon-16.png");

public slots:
      void about() const;

private:
    /*
     * Create new Global QSettings object on heap.
     */
    void initializeSettings();
    void initializeLanguage();
    void initMainWindow();

    QSettings *m_settings = nullptr;
    MainWindow *m_mainWindow = nullptr;
    QTranslator *qtTranslator;
    QTranslator *appTranslator;
};

/**
* Global pointer used to access the Application.
*/
extern Application* application;

#endif // APPLICATION_H
