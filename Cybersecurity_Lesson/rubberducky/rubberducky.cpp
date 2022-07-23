#include "rubberducky.h"
#include "ui_rubberducky.h"

RubberDucky::RubberDucky(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RubberDucky)
{
    ui->setupUi(this);
}

RubberDucky::~RubberDucky()
{
    delete ui;
}
