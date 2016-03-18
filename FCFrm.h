#ifndef FCFRM_H
#define FCFRM_H

#include <QWidget>

namespace Ui {
    class FCFrm;
}

class FCFrm : public QWidget
{
    Q_OBJECT

public:
    explicit FCFrm(QWidget *parent = 0);
    ~FCFrm();

public:
    void setup(QString& str);

private:
    void closeEvent(QCloseEvent *);

private:
    Ui::FCFrm *ui;
};

#endif // FCFRM_H
