#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "views/navigatorview.h"
#include "views/decisionview.h"
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

private:
    Ui::MainWindow *ui;

    NavigatorView *m_navigatorView;
    DecisionView *m_decisionView;

    StatusBarPanel *m_statusBarPanel;
    SearchPanel *m_searchPanel;

    QAction *action_doctype;
    QAction *action_edit;
    QAction *action_new;
    QAction *action_pa;
    QAction *action_print;
    QAction *action_record;
    QAction *action_remove;
    QAction *action_refresh;
    QAction *action_tree;

    //QShortcut *m_editShortcut;
    //QShortcut *m_newShortcut;
    //QShortcut *m_refreshShortcut;
    //QShortcut *m_removeShortcut;
    QShortcut *m_searchShortcut;

    void initialize();
    void restoreAppState();
    void setupShortcuts();
    void setupStatusBar();
    void setupToolBar();

private slots:
    void openDoctype();
    void openConnection();
    void openAuthorities();
    void openRecords();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
