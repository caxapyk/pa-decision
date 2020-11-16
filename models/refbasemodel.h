#ifndef REFBASEMODEL_H
#define REFBASEMODEL_H

#include <QAbstractItemModel>
#include <QStandardItem>

class RefBaseModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    RefBaseModel(QObject *parent = nullptr);
    ~RefBaseModel();

    void setQuery(const QString &query);
    void setRootText(const QString &text);

    //int itemMaxNum(int column, const QRegExp &rule) const;
    //QModelIndex rootItem() const;

    virtual void clear() {};
    virtual void select() {};

    virtual int columnCount(const QModelIndex &parent=QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    //virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    //virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;
    //virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    //virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

private:
    QMap<int, QVariant> columnHeaders;

protected:
    QStandardItem *m_root;
};

#endif // REFBASEMODEL_H
