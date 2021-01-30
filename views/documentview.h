#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include "tablewidgetview.h"

class DocumentView : public TableWidgetView
{
    Q_OBJECT

public:
    DocumentView(QWidget *parent = nullptr);
    ~DocumentView();

    QVariant authorityId() const { return m_authorityId; };
    void setAuthorityId(const QVariant &id) { m_authorityId = id; };

    int total() { return m_total; };

    int from() { return m_from; }
    void setFrom(int from) { m_from = from; }
    int count() { return m_count; }
    void setCount(int count) { m_count = count; }

    void restoreViewState() override;
    void saveViewState() override;

public slots:
    void editRow() override;
    void _insertRow() override;
    void refresh() override;
    void removeRows() override;

private:
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
    void _selected(const QList<QTableWidgetSelectionRange> &ranges);
    void totalChanged(int total);
};

#endif // DOCUMENTVIEW_H
