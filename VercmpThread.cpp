
#include "VercmpThread.h"

#include <QDir>
#include <QProcess>
#include <QDateTime>
#include <QMessageBox>

#include <assert.h>
#define ASSERT(e) assert(e)

#include "ResultFrm.h"
#include "ui_ResultFrm.h"

#define MAX_LISTITEMS 512

VercmpThread::VercmpThread(const QString &strVer1, const QString &strVer2, Ui::ResultFrm *rui, ResultFrm *parent)
    : QThread(parent)
{
    ui = rui;

    m_pFrm = parent;
    m_strVer1 = strVer1;
    m_strVer2 = strVer2;
}

void VercmpThread::run()
{
    //m_strResultDir = qApp->applicationDirPath() + QDateTime::currentDateTime().toString("/yyyy-MM-dd hh.mm.ss.zzz/");
            //QString::number(QDateTime::currentMSecsSinceEpoch());
    //QDir().mkdir(m_strResultDir);
    dc(m_strVer1, m_strVer2);
    QMessageBox::information(m_pFrm, "Done!", "The Vercmp has done!");
}


void VercmpThread::dc(const QString &strDir1, const QString &strDir2)
{
    QFileInfo f1(strDir1), f2(strDir2);
    if (!f1.exists())
    {
        if (!f2.exists())
            return;
        addv2only(showpath(strDir2, 2));
        return;
    }
    if (!f2.exists())
    {
        addv1only(showpath(strDir1, 1));
        return;
    }

    if (f1.isFile())
    {
        if (f2.isDir())
        {
            adddif(showpath(strDir1, 1) + " (typedif)");
            return;
        }
        fc(strDir1, strDir2);
    }
    else
    {
        if (f2.isFile())
        {
            adddif(showpath(strDir1, 1) + " (typedif)");
            return;
        }

        QFileInfoList fl = QDir(strDir1).entryInfoList();
        for (int i = 0; i < fl.size(); ++i)
        {
            QFileInfo& f = fl[i];
            if (f.fileName() == "." || f.fileName() == "..")
                continue;
            dc(f.absoluteFilePath(), m_strVer2 + showpath(f.absoluteFilePath(), 1));
        }

        fl = QDir(strDir2).entryInfoList();

        QFileInfo ft;

        for (int i = 0; i < fl.size(); ++i)
        {
            QFileInfo& f = fl[i];
            if (f.fileName() == "." || f.fileName() == "..")
                continue;
            ft.setFile(m_strVer1 + showpath(f.absoluteFilePath(), 2));
            if (!ft.exists())
                addv2only(showpath(f.absoluteFilePath(), 2));
        }
    }
}

void VercmpThread::fc(const QString &strFile1, const QString &strFile2)
{
    bool same = true;
    FILE *fp1, *fp2;
    fp1 = fopen(strFile1.toLocal8Bit(), "rb");
    //ASSERT(fp1);
    if (!fp1)
    {
        addfailed(showpath(strFile1, 1));
        return;
    }
    fp2 = fopen(strFile2.toLocal8Bit(), "rb");
    //ASSERT(fp2);
    if (!fp2)
    {
        addfailed(showpath(strFile1, 1));
        fclose(fp1);
        return;
    }

    int sz1,sz2;
    fseek(fp1, 0L, SEEK_END);
    sz1 = ftell(fp1);
    fseek(fp2, 0L, SEEK_END);
    sz2 = ftell(fp2);

    if (sz1 != sz2)
        same = false;
    else
    {
        fseek(fp1, 0L, SEEK_SET);
        fseek(fp2, 0L, SEEK_SET);
        int bits1, bits2;
        sz1 >>= 2;
        while (sz1--)
        {
            fread(&bits1, sizeof(int), 1, fp1);
            fread(&bits2, sizeof(int), 1, fp2);
            if (bits1 != bits2)
            {
                same = false;
                break;
            }
        }
        if (same)
        {
            char byte1, byte2;
            sz2 &= 3;
            while (sz2--)
            {
                fread(&byte1, 1, 1, fp1);
                fread(&byte2, 1, 1, fp2);
                if (byte1 != byte2)
                {
                    same = false;
                    break;
                }
            }
        }
    }
    fclose(fp1);
    fclose(fp2);

    if (!same)
    {
        adddif(showpath(strFile1, 1));
    }

    /*
    QString s1(strFile1), s2(strFile2);
    s1.replace('/', '\\');
    s2.replace('/', '\\');

    QProcess p;
    //p.setStandardOutputFile(s);
    p.setNativeArguments("/b \"" + s1 + "\" \"" + s2 + "\"");
    p.start("fc");
    p.waitForFinished(-1);

    if (p.exitCode())
    {
        //QString s = m_strResultDir;
        //s.append(QString::number(ui->difList->count()) + ".txt");
        //s.replace('/', '\\');
        ui->difList->addItem(showpath(s1, 1));
    }
    //QString cmd = "fc /b \"" + s1 + "\" \"" + s2 + "\" >> \"" + s + "\"";
    //system(cmd.toAscii());
    */
}

QString VercmpThread::showpath(const QString &str, int ver)
{
    if (ver == 1)
    {
        return str.right(str.length() - m_strVer1.length());
    }
    else if (ver == 2)
    {
        return str.right(str.length() - m_strVer2.length());
    }
    return "";
}

void VercmpThread::addv1only(const QString &str)
{
    if (ui->v1List->count() < MAX_LISTITEMS)
        ui->v1List->addItem(str);
    else
    {
        FILE *fp = fopen("more Version 1 Only.txt", "ab");
        QByteArray ba = (str + "\n\r").toLocal8Bit();
        fwrite(ba, 1, ba.size(), fp);
        fclose(fp);
    }
}

void VercmpThread::addv2only(const QString &str)
{
    if (ui->v2List->count() < MAX_LISTITEMS)
        ui->v2List->addItem(str);
    else
    {
        FILE *fp = fopen("more Version 2 Only.txt", "ab");
        QByteArray ba = (str + "\n\r").toLocal8Bit();
        fwrite(ba, 1, ba.size(), fp);
        fclose(fp);
    }
}

void VercmpThread::adddif(const QString &str)
{
    if (ui->difList->count() < MAX_LISTITEMS)
        ui->difList->addItem(str);
    else
    {
        FILE *fp = fopen("more Different files.txt", "ab");
        QByteArray ba = (str + "\n\r").toLocal8Bit();
        fwrite(ba, 1, ba.size(), fp);
        fclose(fp);
    }
}

void VercmpThread::addfailed(const QString &str)
{
    if (ui->failedList->count() < MAX_LISTITEMS)
        ui->failedList->addItem(str);
    else
    {
        FILE *fp = fopen("more Failed files.txt", "ab");
        QByteArray ba = (str + "\n\r").toLocal8Bit();
        fwrite(ba, 1, ba.size(), fp);
        fclose(fp);
    }
}
