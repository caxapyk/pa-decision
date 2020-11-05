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

    QAction *actionAuthority() { return m_action_authority; };
    QAction *actionProtocol() { return m_action_protocol; };
    QAction *actionRecord() { return m_action_record; };
    QAction *actionDoctype() { return m_action_doctype; };

private:
    Ui::ReferenceButton *ui;

    QMenu *m_menu;
    QAction *m_action_authority;
    QAction *m_action_protocol;
    QAction *m_action_record;
    QAction *m_action_doctype;
};

#endif // RERERENCEBUTTON_H
