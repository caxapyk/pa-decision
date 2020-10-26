#ifndef STRINGSORT_H
#define STRINGSORT_H

#include <QObject>

class StringSort : public QObject
{
    Q_OBJECT
public:
    explicit StringSort(QObject *parent = nullptr);
    static bool lessThen(QString left, QString right);
};

#endif // STRINGSORT_H
