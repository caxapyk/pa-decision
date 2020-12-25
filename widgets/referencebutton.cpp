#include "referencebutton.h"
#include "ui_referencebutton.h"

ReferenceButton::ReferenceButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReferenceButton)
{
    ui->setupUi(this);

    m_actionDoctype = new QAction(QIcon(":/icons/icons/doctype-16.png"), tr("Document types"));
    m_actionSubjtype = new QAction(QIcon(":/icons/icons/subject-16.png"), tr("Subject types"));

    m_menu = new QMenu;
    m_menu->addAction(m_actionDoctype);
    m_menu->addAction(m_actionSubjtype);

    ui->pB_references->setMenu(m_menu);
}

ReferenceButton::~ReferenceButton()
{
    delete ui;

    delete m_actionDoctype;
    delete m_actionSubjtype;
}
