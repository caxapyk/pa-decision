#include "paginator.h"
#include "ui_paginator.h"

Paginator::Paginator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paginator)
{
    ui->setupUi(this);
}

Paginator::~Paginator()
{
    delete ui;
}
