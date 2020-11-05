#include "referencebutton.h"
#include "ui_referencebutton.h"

ReferenceButton::ReferenceButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReferenceButton)
{
    ui->setupUi(this);

    m_action_authority = new QAction(QIcon(":/icons/icons/icon-16.png"), tr("Public authorities"));
    m_action_protocol = new QAction(QIcon(":/icons/icons/protocol-16.png"), tr("Protocols"));
    m_action_record = new QAction(QIcon(":/icons/icons/record-16.png"), tr("Records"));
    m_action_doctype = new QAction(QIcon(":/icons/icons/doctype-16.png"), tr("Document types"));

    m_menu = new QMenu;
    m_menu->addAction(m_action_authority);
    m_menu->addAction(m_action_protocol);
    m_menu->addAction(m_action_record);
    m_menu->addAction(m_action_doctype);

    ui->pB_references->setMenu(m_menu);
}

ReferenceButton::~ReferenceButton()
{
    delete ui;

    delete m_action_authority;
    delete m_action_protocol;
    delete m_action_record;
    delete m_action_doctype;
}
