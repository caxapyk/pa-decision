#ifndef REFERENCEDIALOGINTERFACE_H
#define REFERENCEDIALOGINTERFACE_H

#include <QDialog>

class ReferenceDialogInterface : public QDialog
{
    Q_OBJECT
public:
    ReferenceDialogInterface(QWidget *parent = nullptr);

public slots:
    virtual void edit() {};
    virtual void insert() {};
    virtual void refresh() {};
    virtual void remove() {};
};

#endif // REFERENCEDIALOGINTERFACE_H
