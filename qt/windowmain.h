#ifndef WINDOWMAIN_H
#define WINDOWMAIN_H

#include <QDialog>
#include "land.h"
#include "head.h"
#include <QListWidgetItem>
#include <QDateTime>
//#include <QtCore>
#include <QTimer>
#include <QTcpSocket>


namespace Ui {
class windowmain;
}

class windowmain : public QDialog
{
    Q_OBJECT

public:
    explicit windowmain(QWidget *parent = 0);
    ~windowmain();
    void masg1();
    void masg2();
    void masg3();
    void closeEvent(QCloseEvent *event);
public  slots:

    void timeupdate();

private slots:

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void camconnectToserver();

    void camreadmessage();

    void on_pushButton_clicked();

    void conses();

    void on_pushButton_2_clicked();


    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_pushButton_3_clicked();

private:
    Ui::windowmain *ui;
    land landui;
    QTimer *timer;
    QTcpSocket ordsocket;
    struct orddata date;
//    QTcpSocket picsocket;
};

#endif // WINDOWMAIN_H
