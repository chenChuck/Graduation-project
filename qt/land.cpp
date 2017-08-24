#include "land.h"
#include "ui_land.h"
extern bool admin;
land::land(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::land)
{
    ui->setupUi(this);
    ui->label->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_2->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_3->setAttribute(Qt::WA_TranslucentBackground);
    setWindowIcon(QIcon(":/picture/Image.png"));
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}
void land::closeEvent(QCloseEvent *event)
{
    exit(0);
}
land::~land()
{
    delete ui;
}

void land::on_pushButton_2_clicked()
{
    exit(0);
}

void land::on_pushButton_clicked()
{
    if(ui->lineEdit->text() == "admin"&&ui->lineEdit_2->text() == "admin"){
        admin = true;
        this->hide();
    }
    else if (ui->lineEdit->text() == "cody"&&ui->lineEdit_2->text() == "123") {
        admin = false;
        this->hide();
    }
    else{
        QMessageBox::warning(this,tr("登陆失败"),tr("请检查账户密码是否正确！"));
    }
}
