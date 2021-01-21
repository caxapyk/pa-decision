#ifndef DETAILSDIALOG_H
#define DETAILSDIALOG_H

#include "models/sqlbasemodel.h"

#include <QDialog>
#include <QDataWidgetMapper>

class DetailsDialog : public QDialog
{
    Q_OBJECT
public:
    DetailsDialog(QWidget *parent = nullptr);
    ~DetailsDialog();

    int id();
    QModelIndex currentIndex() const { return _index; };
    SqlBaseModel *model() { return _model;}
    void setCurrentIndex(const QModelIndex &index) { _index = index; }
    void setModel(SqlBaseModel *model) { _model = model; }

private:
    SqlBaseModel *_model = nullptr;
    QModelIndex _index;
};

#endif // DETAILSDIALOG_H
