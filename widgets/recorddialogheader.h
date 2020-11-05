#ifndef RECORDDIALOGHEADER_H
#define RECORDDIALOGHEADER_H

#include <QWidget>

namespace Ui {
class RecordDialogHeader;
}

class RecordDialogHeader : public QWidget
{
    Q_OBJECT

public:
    explicit RecordDialogHeader(QWidget *parent = nullptr);
    ~RecordDialogHeader();

    void setFundName(const QString &name);

private:
    Ui::RecordDialogHeader *ui;
};

#endif // RECORDDIALOGHEADER_H
