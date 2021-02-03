#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include "tablewidgetview.h"
#include "math.h"

class DocumentView : public TableWidgetView
{
    Q_OBJECT

public:
    DocumentView(QWidget *parent = nullptr);
    ~DocumentView();

    QVariant authorityId() const { return m_authorityId; };
    void setAuthorityId(const QVariant &id) { m_authorityId = id; };

    int total() { return m_total; };
    int page() { return ceil(static_cast<double>(m_from)/static_cast<double>(m_count)); };
    int totalPages() { return ceil(static_cast<double>(m_total)/static_cast<double>(m_count));  };

    void restoreViewState() override;
    void saveViewState() override;

public slots:
    void editRow() override;
    void _insertRow() override;
    void refresh() override;
    void removeRows() override;

    void backward();
    void toward();
    void nextPage();
    void previousPage();
    void gotoPage(int page);
    void perPage(int count);

private:
    QAction *m_viewAction;
    QStringList m_headerLabels = {tr("ID"), tr("Number"), tr("Name"), tr("Comment")};

    Qt::SortOrder m_order = Qt::AscendingOrder;
    QVariant m_authorityId;

    int m_from = 0;
    int m_count = 20;
    int m_total = -1;

    void updateTotal();
    void contextMenu(BaseContextMenu &menu) override;

private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;
    void sort(int section, Qt::SortOrder order);

signals:
    void viewDocument(int row);
    void totalChanged(int total);
    void refreshed();
};

#endif // DOCUMENTVIEW_H
