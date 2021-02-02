#ifndef SUBJECTVIEW_H
#define SUBJECTVIEW_H

#include "tablewidgetview.h"

class SubjectView : public TableWidgetView
{
    Q_OBJECT
public:
    SubjectView(QWidget *parent = nullptr);
    ~SubjectView();

    void contextMenu(BaseContextMenu &menu) override;
    void setDocumentId(const QVariant &id) { m_docId = id; }

    void refresh() override;
    bool save();
    bool validate();

public slots:
    void _insertRow() override;
    void removeRows() override;

private:
    QStringList m_subjectHeaderLabels = {"Action", tr("ID"), tr("Type"), tr("Name"), tr("Address"), tr("Description"), tr("Pages")};
    QVariant m_docId;

    void restoreViewState() override;
    void saveViewState() override;

private slots:
    void _itemChanged(QTableWidgetItem *item);
};

#endif // SUBJECTVIEW_H
