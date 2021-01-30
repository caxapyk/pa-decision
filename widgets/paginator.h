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

    void setCurrentPage(int page);
    int currentPage() { return m_currentPage; }

    void setTotalPages(int pages);
    int totalPages() { return m_totalPages; }

    void setTowardEnabled(bool ok);
    void setBackwardEnabled(bool ok);
    void setNextEnabled(bool ok);
    void setPreviousEnabled(bool ok);

private:
    Ui::Paginator *ui;

    int m_currentPage = 1;
    int m_totalPages = 1;

signals:
    void toward();
    void backward();
    void nextPage();
    void previousPage();
    void gotoPage(int page);
    void perPageChanged(int rows);
};

#endif // PAGINATOR_H
