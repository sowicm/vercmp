#include "ResultFrm.h"
#include "ui_ResultFrm.h"

#include <QClipboard>
#include <QMessageBox>

#include "VercmpThread.h"
#include "FCFrm.h"


ResultFrm::ResultFrm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultFrm)
{
    ui->setupUi(this);

    m_pVcThread = NULL;
}

ResultFrm::~ResultFrm()
{
    if (m_pVcThread)
        delete m_pVcThread;

    delete ui;
}

void ResultFrm::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);

    delete this;
}

void ResultFrm::setup(const QString &strVer1, const QString &strVer2)
{
    ui->v1Edit->setText(m_strVer1 = strVer1);
    ui->v2Edit->setText(m_strVer2 = strVer2);
    m_pVcThread = new VercmpThread(strVer1, strVer2, ui, this);
    m_pVcThread->start(QThread::HighestPriority);
    show();
}


void ResultFrm::on_v1List_doubleClicked(const QModelIndex &index)
{

}

void ResultFrm::on_v2List_doubleClicked(const QModelIndex &index)
{

}

void ResultFrm::on_difList_doubleClicked(const QModelIndex &index)
{
    //QProcess::start("open", m_strResultDir + QString::number(index.internalId()) + ".txt");
    //QProcess p;
    //p.start(m_strResultDir + QString::number(index.internalId()) + ".txt");


    QString s1 = m_strVer1 + ui->difList->item(index.row())->text();
    QString s2 = m_strVer2 + ui->difList->item(index.row())->text();
    s1.replace('/', '\\');
    s2.replace('/', '\\');

#if 1
    QProcess p;
    //p.setStandardOutputProcess();
    //p.setNativeArguments("/b \"" + s1 + "\" \"" + s2 + "\"");
    p.start("fc", QStringList() << s1 << s2);
    p.waitForFinished(-1);

    FCFrm *frm = new FCFrm();
    QString s = p.readAll();
    frm->setup(s);
    /*
    if (p.exitCode())
    {
        //QString s = m_strResultDir;
        //s.append(QString::number(ui->difList->count()) + ".txt");
        //s.replace('/', '\\');
        ui->difList->addItem(showpath(s1, 1));
    }
    */
#else
    QString cmd = "fc \"" + s1 + "\" \"" + s2 + "\"";
    system(cmd.toAscii());
#endif
}



void ResultFrm::on_cpButton1_clicked()
{
    QString str = "Version 1 Only\n\r";
    for (int i = 0; i < ui->v1List->count(); ++i)
    {
        str.append(ui->v1List->item(i)->text() + "\n\r");
    }
    QApplication::clipboard()->setText(str);
    QMessageBox::information(this, "Done!", "These contents have been copied to clipboard.");
}

void ResultFrm::on_cpButton2_clicked()
{
    QString str = "Different files\n\r";
    for (int i = 0; i < ui->difList->count(); ++i)
    {
        str.append(ui->difList->item(i)->text() + "\n\r");
    }
    QApplication::clipboard()->setText(str);
    QMessageBox::information(this, "Done!", "These contents have been copied to clipboard.");
}

void ResultFrm::on_cpButton3_clicked()
{
    QString str = "Version 2 Only\n\r";
    for (int i = 0; i < ui->v2List->count(); ++i)
    {
        str.append(ui->v2List->item(i)->text() + "\n\r");
    }
    QApplication::clipboard()->setText(str);
    QMessageBox::information(this, "Done!", "These contents have been copied to clipboard.");
}

void ResultFrm::on_cpButton4_clicked()
{
    QString str = "Failed files\n\r";
    for (int i = 0; i < ui->failedList->count(); ++i)
    {
        str.append(ui->failedList->item(i)->text() + "\n\r");
    }
    QApplication::clipboard()->setText(str);
    QMessageBox::information(this, "Done!", "These contents have been copied to clipboard.");
}

