#include "colorrect.h"

ColorRect::ColorRect(QWidget *parent) : QWidget(parent)
{

}

QPixmap ColorRect::pixmap(const QColor &color, int size)
{
    QPixmap pixmap(size, size - 5);

    pixmap.fill(color);
    QRect border(0, 0, size - 1, size - 6);
    QPainter painter(&pixmap);
    QPen pen(Qt::gray, 1, Qt::SolidLine);
    painter.setPen(pen);
    painter.drawRect(border);

    return pixmap;
}
