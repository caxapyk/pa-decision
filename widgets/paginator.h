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

    void setTowardEnabled(bool ok);
    void setBackwardEnabled(bool ok);
    void setNextEnabled(bool ok);
    void setPreviousEnabled(bool ok);

private:
    Ui::Paginator *ui;

signals:
    void toward();
    void backward();
    void nextPage();
    void previousPage();
    void gotoPage(int page);
    void showPerPage(int rows);
};

#endif // PAGINATOR_H
