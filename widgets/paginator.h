#ifndef PAGINATOR_H
#define PAGINATOR_H

#include <QWidget>

namespace Ui {
class Paginator;
}

class Paginator : public QWidget
{
    Q_OBJECT

public:
    explicit Paginator(QWidget *parent = nullptr);
    ~Paginator();

private:
    Ui::Paginator *ui;
};

#endif // PAGINATOR_H
