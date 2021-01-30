#ifndef NAVPANEL_H
#define NAVPANEL_H

#include <QWidget>
#include <QHBoxLayout>
#include <QButtonGroup>

namespace Ui {
class NavPanel;
}

class NavPanel : public QWidget
{
    Q_OBJECT

public:
    explicit NavPanel(QWidget *parent = nullptr);
    ~NavPanel();

    QHBoxLayout *_layout();

    void setHorizontalChecked();
    void setVerticalChecked();

public slots:
    void setTotal(int total);

private:
    Ui::NavPanel *ui;
    QButtonGroup *m_orientation;

signals:
    void orientationChanged(Qt::Orientation);
};

#endif // NAVPANEL_H
