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

    void contextMenu(BaseContextMenu &menu) override;
    void restoreViewState() override;
    void saveViewState() override;

public slots:
    void editRow(int row) override;
    void insertRow(int row = 0) override;
    void refresh() override;
    void removeRows(const QList<QTableWidgetSelectionRange> &ranges) override;

private:
    QStringList m_headerLabels = {tr("ID"), tr("Number"), tr("Name"), tr("Comment")};

    Qt::SortOrder m_order = Qt::AscendingOrder;
    QVariant m_authorityId;
    int m_total = -1;

    void updateTotal(int count);

private slots:
    void sort(int section, Qt::SortOrder order);
};

#endif // DOCUMENTVIEW_H
