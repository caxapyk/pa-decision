#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "views/view.h"
#include "views/authoritytreeview.h"
#include "views/explorerview.h"
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

    ExplorerView *explorer() { return m_explorerView; };

    QAction *action_edit;
    QAction *action_new;
    QAction *action_print;
    QAction *action_remove;
    QAction *action_refresh;
    QAction *action_tree;
    QAction *action_record;

public slots:
    void openDialog(QDialog *dialog);

private:
    Ui::MainWindow *ui;

    AuthorityTreeView *m_authorityTreeView;
    ExplorerView *m_explorerView;

    ReferenceButton *m_referenceButton;
    StatusBarPanel *m_statusBarPanel;
    //SearchPanel *m_searchPanel;

    //QShortcut *m_searchShortcut;

    void initialize();

    void restoreAppState();
    void saveAppState();

    //void openInNewTab(const QModelIndex &index);

    void setupShortcuts();
    void setupStatusBar();
    void setupToolBar();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
