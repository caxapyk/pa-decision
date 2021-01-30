#ifndef TAB_H
#define TAB_H

#include <QVBoxLayout>
#include <QWidget>

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = nullptr);
    ~Tab();

    QBoxLayout *_layout() { return  m_layout; };

private:
    QVBoxLayout *m_layout;
};

#endif // TAB_H
