#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "views/authorityview.h"
#include "widgets/explorer.h"
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

    QAction *editAction() { return m_actionEdit; };
    QAction *insertAction() { return m_actionInsert; };
    QAction *printAction() { return m_actionPrint; };
    QAction *removeAction() { return m_actionRemove; };
    QAction *refreshAction() { return m_actionRefresh; };
    QAction *treeAction() { return m_actionTree; };
    QAction *afAction() { return m_actionAF; };

    QWidget *searchPanel() { return m_searchPanel; };

public slots:
    void openDialog(QDialog *dialog);

private:
    Ui::MainWindow *ui;

    AuthorityView *m_authorityView;
    Explorer *m_explorer;

    ReferenceButton *m_referenceButton;
    SearchPanel *m_searchPanel;
    StatusBarPanel *m_statusBarPanel;

    QAction *m_actionEdit;
    QAction *m_actionInsert;
    QAction *m_actionPrint;
    QAction *m_actionRemove;
    QAction *m_actionRefresh;
    QAction *m_actionTree;
    QAction *m_actionAF;

    QShortcut *m_searchShortcut;

    void initialize();

    void restoreAppState();
    void saveAppState();

    void setupShortcuts();
    void setupStatusBar();
    void setupToolBar();

private slots:
    void openAF();
    void openAuthorityInNewTab(const QVariant &id);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
