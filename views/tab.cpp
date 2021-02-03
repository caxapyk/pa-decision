#include "tab.h"

#include <QDebug>

Tab::Tab(QWidget *parent) : View(parent)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);
}

Tab::~Tab()
{
    delete m_layout;
}
