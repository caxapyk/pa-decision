#ifndef SUBJECTTYPEDELEGATE_H
#define SUBJECTTYPEDELEGATE_H

#include <QStyledItemDelegate>
#include <QSqlQuery>

class SubjectDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    SubjectDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                         const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor,
            const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QSqlQuery query;
    QMap<QVariant, QVariant> m_doctype;
};

#endif // SUBJECTTYPEDELEGATE_H
