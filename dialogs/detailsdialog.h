#ifndef DETAILSDIALOG_H
#define DETAILSDIALOG_H

#include "models/referencemodel.h"

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
    ReferenceModel *model() { return _model;}
    void setCurrentIndex(const QModelIndex &index) { _index = index; }
    void setModel(ReferenceModel *model) { _model = model; }

private:
    ReferenceModel *_model = nullptr;
    QModelIndex _index;
};

#endif // DETAILSDIALOG_H
