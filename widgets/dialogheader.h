#ifndef RECORDDIALOGHEADER_H
#define RECORDDIALOGHEADER_H

#include "models/authorityflatmodel.h"

#include <QWidget>

namespace Ui {
class DialogHeader;
}

class DialogHeader : public QWidget
{
    Q_OBJECT

public:
    explicit DialogHeader(QWidget *parent = nullptr);
    ~DialogHeader();

    void setFundName(const QString &name);
    int id() { return currentId; };

signals:
    void authorityChanged(int id);

private:
    Ui::DialogHeader *ui;
    AuthorityFlatModel m_authorityModel;

    int currentId;

private slots:
    void currentIndexChanged(int index);
};

#endif // RECORDDIALOGHEADER_H
