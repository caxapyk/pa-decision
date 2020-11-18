#include "tab.h"

#include <QDebug>

Tab::Tab(QWidget *parent) : QWidget(parent)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);
}

Tab::~Tab()
{
    delete m_layout;
}
