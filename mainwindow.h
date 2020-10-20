#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "views/authorityview.h"
#include "views/decisionview.h"

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

    AuthorityView *m_authorityView;
    DecisionView *m_decisionView;

    void initialize();
    void restoreAppState();

private slots:
    void openConnection();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
