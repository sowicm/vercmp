#include "FCFrm.h"
#include "ui_FCFrmm.h"

FCFrm::FCFrm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FCFrm)
{
    ui->setupUi(this);
}

FCFrm::~FCFrm()
{
    delete ui;
}

void FCFrm::setup(QString &str)
{
    ui->edit->setText(str);
    show();
}

void FCFrm::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);

    delete this;
}
