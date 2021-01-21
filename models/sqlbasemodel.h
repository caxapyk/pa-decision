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

    int authorityId() { return m_authorityId; };
    void setAuthorityId(int id) { m_authorityId = id; };

    void andWhere(const QString &condition);
    void orWhere(const QString &condition);
    void where(const QString &condition);

    void clearFilter() { m_filter.clear(); };

    QString filter() const { return m_filter; };

private:
    int m_authorityId = 0;
    QString m_filter;

protected:
    int itemMaxNum(int column, const QRegExp &rule) const;
};

#endif // ReferenceModel_H
