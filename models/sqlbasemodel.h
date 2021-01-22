#ifndef ReferenceModel_H
#define ReferenceModel_H

#include <QAbstractItemModel>

class SqlBaseModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit SqlBaseModel(QObject *parent = nullptr);

    virtual void clear() = 0;
    virtual void select() = 0;

    QVariant authorityId() const { return m_authorityId; };
    void setAuthorityId(int id) { m_authorityId = id; };

    void andWhere(const QString &condition);
    void orWhere(const QString &condition);
    void where(const QString &condition);

    void clearFilter() { m_filter.clear(); };

    QString filter() const { return m_filter; };

private:
    QVariant m_authorityId;
    QString m_filter;
};

#endif // ReferenceModel_H
