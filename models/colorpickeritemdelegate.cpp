#include "colorpickeritemdelegate.h"
#include "widgets/colorrect.h"

#include <QDebug>
#include <QComboBox>

ColorPickerItemDelegate::ColorPickerItemDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *ColorPickerItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    QComboBox *editor = new QComboBox(parent);
    QPixmap pixmap;
    QStringList colors;

    colors << "white" << "antiquewhite" << "azure" << "khaki" << "lavender" << "lightyellow" << "peachpuff" << "pink";

    for (int i = 0; i < colors.size(); ++i) {
        QString color = colors.at(i);
        QPixmap pixmap = ColorRect::pixmap(QColor(color), editor->style()->pixelMetric(QStyle::PM_SmallIconSize));

        editor->addItem(color);
        editor->setItemData(i, pixmap, Qt::DecorationRole);

        if(index.data().toString() == color)
            editor->setCurrentIndex(i);
    }

    return editor;
}
