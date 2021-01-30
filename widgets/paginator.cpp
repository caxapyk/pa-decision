#include "paginator.h"
#include "ui_paginator.h"

Paginator::Paginator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paginator)
{
    ui->setupUi(this);

    connect(ui->pB_toward, &QPushButton::clicked, this, &Paginator::toward);
    connect(ui->pB_backward, &QPushButton::clicked, this, &Paginator::backward);
    connect(ui->pB_next, &QPushButton::clicked, this, &Paginator::nextPage);
    connect(ui->pB_previous, &QPushButton::clicked, this, &Paginator::previousPage);

    connect(ui->pB_gotoPage, &QPushButton::clicked, this, [=] {
        emit gotoPage(ui->sB_gotoPage->value());
    });

    connect(ui->cB_perPage, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [=] {
        emit showPerPage(QString(ui->cB_perPage->currentText()).toInt());
    });
}

Paginator::~Paginator()
{
    delete ui;
}

void Paginator::setTowardEnabled(bool ok)
{
    ui->pB_toward->setEnabled(ok);
}

void Paginator::setBackwardEnabled(bool ok)
{
    ui->pB_backward->setEnabled(ok);
}

void Paginator::setNextEnabled(bool ok)
{
    ui->pB_next->setEnabled(ok);
}

void Paginator::setPreviousEnabled(bool ok)
{
    ui->pB_previous->setEnabled(ok);
}
