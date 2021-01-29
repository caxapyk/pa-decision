#include "referencedialog.h"

#include <QPushButton>

ReferenceDialog::ReferenceDialog(QWidget *parent) : QDialog(parent)
{
    m_layout = new QVBoxLayout;
    setLayout(m_layout);

    m_bbox = new QDialogButtonBox(QDialogButtonBox::Ok);
    m_layout->addWidget(m_bbox);
    connect(m_bbox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &ReferenceDialog::accept);
}

ReferenceDialog::~ReferenceDialog()
{
    delete m_layout;
    delete m_bbox;
}
