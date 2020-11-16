#ifndef ReferenceModel_H
#define ReferenceModel_H

#include <QAbstractItemModel>

class ReferenceModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ReferenceModel(QObject *parent = nullptr);

    virtual QVariant headerData(int, Qt::Orientation, int role = Qt::DisplayRole) const = 0;
    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const = 0;
    virtual QModelIndex parent(const QModelIndex &index) const = 0;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const = 0;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const = 0;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const = 0;
    virtual bool setRow(int row, QMap<QString, QVariant> &values, const QModelIndex &parent=QModelIndex()) { return false ;};

    virtual void clear() = 0;
    virtual void select() = 0;

    int authorityId() { return m_authorityId; };
    void setAuthorityId(int id) { m_authorityId = id; };

    void andWhere(const QString &condition);
    void orWhere(const QString &condition);
    void where(const QString &condition);

    void clearFilter() { cond = QString(); };

    QString filter() { return cond; };

private:
    int m_authorityId = 0;
    QString cond;

protected:
    int itemMaxNum(int column, const QRegExp &rule) const;

};

#endif // ReferenceModel_H
