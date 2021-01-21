#ifndef ITEMCOUNTER_H
#define ITEMCOUNTER_H

#include <QObject>
#include <QAbstractItemModel>

class ItemCounter : public QObject
{
    Q_OBJECT
public:
    explicit ItemCounter(QAbstractItemModel *model = nullptr, QObject *parent = nullptr);
    void setModel(QAbstractItemModel *model) { m_model = model; };

    int next(int column, const QRegExp &rule) const;

private:
    QAbstractItemModel *m_model;
};

#endif // ITEMCOUNTER_H
