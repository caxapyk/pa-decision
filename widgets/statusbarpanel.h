#ifndef STATUSBARPANEL_H
#define STATUSBARPANEL_H

#include <QWidget>

namespace Ui {
class StatusBarPanel;
}

class StatusBarPanel : public QWidget
{
    Q_OBJECT

public:
    explicit StatusBarPanel(QWidget *parent = nullptr);
    ~StatusBarPanel();

private:
    Ui::StatusBarPanel *ui;
};

#endif // STATUSBARPANEL_H
