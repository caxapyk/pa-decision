#include "subjectdelegate.h"

#include <QComboBox>
#include <QPlainTextEdit>

SubjectDelegate::SubjectDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    query.prepare("select id, name from pad_subjtype");
    query.exec();

    while(query.next()) {
        m_doctype.insert(query.value(0), query.value(1));
    }
}


QWidget *SubjectDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 2) {
        QComboBox *editor = new QComboBox(parent);
        return editor;
    } else if(index.column() == 4 || index.column() == 5) {
        QPlainTextEdit *editor = new QPlainTextEdit(parent);
        return editor;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);

}

void SubjectDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if(index.column() == 2) {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);

        QMapIterator<QVariant, QVariant> i(m_doctype);
         while (i.hasNext()) {
             i.next();

             QString val = i.value().toString();
             comboBox->addItem(val);

             if(val == index.data().toString())
                 comboBox->setCurrentText(val);
         }
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void SubjectDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if(index.column() == 2) {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QVariant key = m_doctype.key(comboBox->itemText(comboBox->currentIndex()));
        QVariant value = m_doctype.value(key);

        model->setData(index, value, Qt::EditRole);
        model->setData(index, key, Qt::UserRole);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void SubjectDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 4 || index.column() == 5) {
        QRect rect = option.rect;
        rect.setHeight(rect.height() * 2);

        editor->setGeometry(rect);
    } else {
        editor->setGeometry(option.rect);
    }
}
