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

public slots:
    void insertRow(int row) override;
    void removeRow(int row) override;

private:
    QStringList m_subjectHeaderLabels = {"Action", tr("ID"), tr("Type"), tr("Name"), tr("Address"), tr("Description")};
};

#endif // SUBJECTVIEW_H
