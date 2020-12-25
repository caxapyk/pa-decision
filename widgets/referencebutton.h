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

    QAction *actionProtocol() { return m_actionProtocol; };
    QAction *actionRecord() { return m_actionRecord; };
    QAction *actionDoctype() { return m_actionDoctype; };
    QAction *actionSubject() { return m_actionSubject; };

private:
    Ui::ReferenceButton *ui;

    QMenu *m_menu;
    QAction *m_actionProtocol;
    QAction *m_actionRecord;
    QAction *m_actionDoctype;
    QAction *m_actionSubject;
};

#endif // RERERENCEBUTTON_H
