#ifndef SEARCHPANEL_H
#define SEARCHPANEL_H

#include <QWidget>

namespace Ui {
class SearchPanel;
}

class SearchPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SearchPanel(QWidget *parent = nullptr);
    ~SearchPanel();
    void setFocus();

private:
    Ui::SearchPanel *ui;
};

#endif // SEARCHPANEL_H
