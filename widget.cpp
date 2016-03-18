#include "widget.h"
#include "ui_widget.h"

#include "ResultFrm.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_pCompleter = new QCompleter(this);
    m_pCompleter->setMaxVisibleItems(9);
    QFileSystemModel *fsModel = new QFileSystemModel(m_pCompleter);
    fsModel->setRootPath("");
    m_pCompleter->setModel(fsModel);
    m_pCompleter->setCompletionMode(QCompleter::PopupCompletion);
    m_pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_pCompleter->setWrapAround(true);

    ui->d1Edit->setCompleter(m_pCompleter);
    ui->d2Edit->setCompleter(m_pCompleter);
}

Widget::~Widget()
{
    delete m_pCompleter;
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, QString(), QString(), QFileDialog::ShowDirsOnly);
    if (!fileName.isEmpty())
    {
        ui->d1Edit->setText(fileName);
    }
}

void Widget::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this, QString(), QString(), QFileDialog::ShowDirsOnly);
    if (!fileName.isEmpty())
    {
        ui->d2Edit->setText(fileName);
    }
}

void Widget::on_pushButton_3_clicked()
{
    ResultFrm *pFrm = new ResultFrm();
    pFrm->setup(ui->d1Edit->text(), ui->d2Edit->text());
}
