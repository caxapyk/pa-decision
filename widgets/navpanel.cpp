#include "navpanel.h"
#include "ui_navpanel.h"

NavPanel::NavPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NavPanel)
{
    ui->setupUi(this);

    m_orientation = new QButtonGroup;
    m_orientation->setExclusive(true);

    m_orientation->addButton(ui->pB_horizontal);
    m_orientation->addButton(ui->pB_vertical);

    connect(m_orientation, QOverload<QAbstractButton *, bool>::of(&QButtonGroup::buttonToggled), this, [=] (QAbstractButton *button, bool checked) {
        if(button == ui->pB_horizontal && checked)
            emit orientationChanged(Qt::Horizontal);
        else if(button == ui->pB_vertical && checked)
            emit orientationChanged(Qt::Vertical);
    });
}

NavPanel::~NavPanel()
{
    delete ui;
}

QHBoxLayout *NavPanel::_layout()
{
    return ui->hL_panel;
}

void NavPanel::setHorizontalChecked()
{
    ui->pB_horizontal->setChecked(true);
}

void NavPanel::setVerticalChecked()
{
    ui->pB_vertical->setChecked(true);
}

void NavPanel::setTotal(int total)
{
    ui->lcdn_counter->display(total);
}
