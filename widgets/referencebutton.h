#ifndef RERERENCEBUTTON_H
#define RERERENCEBUTTON_H

#include <QMenu>
#include <QWidget>

namespace Ui {
class ReferenceButton;
}

class ReferenceButton : public QWidget
{
    Q_OBJECT

public:
    explicit ReferenceButton(QWidget *parent = nullptr);
    ~ReferenceButton();

    QAction *actionDoctype() { return m_actionDoctype; };
    QAction *actionSubjtype() { return m_actionSubjtype; };

private:
    Ui::ReferenceButton *ui;

    QMenu *m_menu;
    QAction *m_actionDoctype;
    QAction *m_actionSubjtype;
};

#endif // RERERENCEBUTTON_H
