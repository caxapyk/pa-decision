#ifndef VIEW_H
#define VIEW_H

#include <QWidget>

class View : public QWidget
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = nullptr);

    void virtual restoreViewState(){};
    void virtual saveViewState(){};

protected:
    void virtual initialize(){};
};

#endif // VIEW_H
