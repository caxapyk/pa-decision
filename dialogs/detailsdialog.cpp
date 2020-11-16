#include "detailsdialog.h"

DetailsDialog::DetailsDialog(QWidget *parent) : QDialog(parent)
{
}

DetailsDialog::~DetailsDialog()
{
}

int DetailsDialog::id()
{
    if(currentIndex().isValid()) {
        return currentIndex().siblingAtColumn(0).data().toInt();
    }

    return -1;
}
