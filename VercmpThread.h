#ifndef VERCMPTHREAD_H
#define VERCMPTHREAD_H

#include <QtCore>

namespace Ui {
    class ResultFrm;
}
class ResultFrm;

class VercmpThread : public QThread
{
public:
    VercmpThread(const QString &strVer1, const QString &strVer2, Ui::ResultFrm *rui, ResultFrm *parent);

    void run();

public:
    void dc(const QString &strDir1, const QString &strDir2);
    void fc(const QString &strFile1, const QString &strFile2);

    QString showpath(const QString &str, int ver);

    void addv1only(const QString &str);
    void addv2only(const QString &str);
    void adddif(const QString &str);
    void addfailed(const QString &str);

private:
    ResultFrm *m_pFrm;
    QString m_strVer1, m_strVer2;
    //QString m_strResultDir;

private:
    Ui::ResultFrm *ui;
};


#endif // VERCMPTHREAD_H
