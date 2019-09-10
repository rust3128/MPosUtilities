#include "viewprogressform.h"
#include "ui_viewprogressform.h"

ViewProgressForm::ViewProgressForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewProgressForm)
{
    ui->setupUi(this);
}

ViewProgressForm::~ViewProgressForm()
{
    delete ui;
}
