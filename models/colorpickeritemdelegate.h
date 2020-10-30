#ifndef COLORPICKERITEMDELEGATE_H
#define COLORPICKERITEMDELEGATE_H

#include <QItemDelegate>

class ColorPickerItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ColorPickerItemDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // COLORPICKERITEMDELEGATE_H
