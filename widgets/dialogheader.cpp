#include "dialogheader.h"
#include "ui_dialogheader.h"

#include <QDebug>

DialogHeader::DialogHeader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogHeader)
{
    ui->setupUi(this);
    connect(ui->cB_authority, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DialogHeader::currentIndexChanged);

    m_authorityModel.select();
    ui->cB_authority->setModel(&m_authorityModel);
}

DialogHeader::~DialogHeader()
{
    disconnect(ui->cB_authority, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DialogHeader::currentIndexChanged);
    delete ui;
}

void DialogHeader::currentIndexChanged(int index)
{
    QVariant data = m_authorityModel.data(m_authorityModel.index(index, 0), ReferenceModel::IDRole);

    if(data.isValid()) {
        currentId = data.toInt();
        emit authorityChanged(currentId);
    }
}

void DialogHeader::setFundName(const QString &name)
{
    ui->label_fundName->setText(name);
}
