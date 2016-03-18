#ifndef RESULTFRM_H
#define RESULTFRM_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
    class ResultFrm;
}

class VercmpThread;

class ResultFrm : public QWidget
{
friend class VercmpThread;
    Q_OBJECT

public:
    explicit ResultFrm(QWidget *parent = 0);
    ~ResultFrm();

public:
    void setup(const QString &strVer1, const QString &strVer2);

private:
    VercmpThread    *m_pVcThread;
    QString          m_strVer1, m_strVer2;

private slots:
    void on_v1List_doubleClicked(const QModelIndex &index);

    void on_v2List_doubleClicked(const QModelIndex &index);

    void on_difList_doubleClicked(const QModelIndex &index);

    void on_cpButton1_clicked();

    void on_cpButton2_clicked();

    void on_cpButton3_clicked();

    void on_cpButton4_clicked();

private:
    void closeEvent(QCloseEvent *);

private:
    Ui::ResultFrm *ui;
};

#endif // RESULTFRM_H
