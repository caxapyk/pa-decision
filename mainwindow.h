#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "views/navigatorview.h"
#include "views/decisionview.h"
#include "widgets/referencebutton.h"
#include "widgets/statusbarpanel.h"
#include "widgets/searchpanel.h"

#include <QShortcut>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QAction *action_edit;
    QAction *action_new;
    QAction *action_print;
    QAction *action_remove;
    QAction *action_refresh;
    QAction *action_tree;

public slots:
    void openDoctype();
    void openConnection();
    void openAuthorities();
    void openProtocol();
    void openRecords();

private:
    Ui::MainWindow *ui;

    NavigatorView *m_navigatorView;
    DecisionView *m_decisionView;

    ReferenceButton *m_referenceButton;
    StatusBarPanel *m_statusBarPanel;
    SearchPanel *m_searchPanel;

    QShortcut *m_searchShortcut;

    void initialize();
    void restoreAppState();
    void setupShortcuts();
    void setupStatusBar();
    void setupToolBar();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
