#ifndef DECISIONPROXYMODEL_H
#define DECISIONPROXYMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

class DecisionProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    DecisionProxyModel(QObject *parent = nullptr);
};

#endif // DECISIONPROXYMODEL_H
