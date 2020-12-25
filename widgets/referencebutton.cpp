#include "referencebutton.h"
#include "ui_referencebutton.h"

ReferenceButton::ReferenceButton(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReferenceButton)
{
    ui->setupUi(this);

    m_actionProtocol = new QAction(QIcon(":/icons/icons/protocol-16.png"), tr("Protocols"));
    m_actionRecord = new QAction(QIcon(":/icons/icons/record-16.png"), tr("Records"));
    m_actionDoctype = new QAction(QIcon(":/icons/icons/doctype-16.png"), tr("Document types"));
    m_actionSubject = new QAction(QIcon(":/icons/icons/subject-16.png"), tr("Subjects"));

    m_menu = new QMenu;
    m_menu->addAction(m_actionProtocol);
    m_menu->addAction(m_actionRecord);
    m_menu->addAction(m_actionDoctype);
    m_menu->addAction(m_actionSubject);

    ui->pB_references->setMenu(m_menu);
}

ReferenceButton::~ReferenceButton()
{
    delete ui;

    delete m_actionProtocol;
    delete m_actionRecord;
    delete m_actionDoctype;
    delete m_actionSubject;
}
