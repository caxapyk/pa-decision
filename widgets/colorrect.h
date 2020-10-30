#ifndef COLORRECT_H
#define COLORRECT_H

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QStyle>
#include <QWidget>

class ColorRect : public QWidget
{
    Q_OBJECT

public:
    explicit ColorRect(QWidget *parent = nullptr);
    static QPixmap pixmap(const QColor &color, int size);
};

#endif // COLORRECT_H
