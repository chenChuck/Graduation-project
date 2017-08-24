#include "windowmain.h"
#include "ui_windowmain.h"
extern bool admin;
windowmain::windowmain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::windowmain)
{
    landui.exec();
    setWindowIcon(QIcon(":/picture/Image.png"));

    ui->setupUi(this);
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");//设置系统时间显示格式
    ui->label_2->setText(str);//在标签上显示时间
    timer = new QTimer();
    timer->setInterval(1000);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(timeupdate()));
    connect(&ordsocket,SIGNAL(connected()),this,SLOT(conses()));
    connect(&ordsocket,SIGNAL(readyRead()),this,SLOT(camreadmessage()));
    camconnectToserver();
    ui->label->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_2->setAttribute(Qt::WA_TranslucentBackground);
    ui->label_3->setAttribute(Qt::WA_TranslucentBackground);
    if(!admin)
    {
        ui->pushButton_2->hide();
        ui->pushButton_3->hide();
    }
}

windowmain::~windowmain()
{
    delete ui;
}

#if 1
void windowmain::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_2->blockSignals(true);
    QString ad = ui->comboBox->currentText();
    ui->label->clear();
    if(ad == "选择日期")
    {
        ui->comboBox_2->clear();
        QString a = "选择图像";
        QStringList tmp = QStringList() << a;
        ui->comboBox_2->addItems(tmp);
        return;
    }
    ui->comboBox_2->clear();
    QString a = "选择图像";
    QStringList tmp = QStringList() << a;
    ui->comboBox_2->addItems(tmp);
    memset(&date,0,sizeof(date));
    date.mode = 1;
    a = ui->comboBox->currentText();
    QByteArray ba = a.toLatin1();
    strcpy(date.buff[0],ba.data());
   // qDebug () << a;
    ordsocket.write((char *)&date,sizeof(date));
    ui->comboBox_2->blockSignals(false);
}

//#elif 0

void windowmain::camconnectToserver()
{
    ordsocket.connectToHost("192.168.84.132",8888);
}

void windowmain::camreadmessage()
{
    memset(&date,0,sizeof(date));
    ordsocket.read((char *)&date,8);
    switch(date.mode)
    {
        case GETDATELIST:
            masg1();
            qDebug()<< 1;
            break;
        case GETPICLIST:
            qDebug()<< 2;
            masg2();
            break;
        case GETPICTURE:
            qDebug()<< 3;
            masg3();
    }
}

void windowmain::masg1()
{
#if 1

    ordsocket.read((char *)&date.buff,sizeof(date)-8);
    int num = date.num;
    int n = 1;

    while (num--) {
        char *buff;
        buff = date.buff[n++];
        QString tmp = QString(QLatin1String(buff));
        QStringList addtext = QStringList() << tmp;
        ui->comboBox->addItems(addtext);
   }
#endif
}

void windowmain::masg2()
{
    ordsocket.read((char *)&date.buff,sizeof(date)-8);
    int num = date.num;
    int n = 1;

    while (num--) {
        char *buff;
        buff = date.buff[n++];
        QString tmp = QString(QLatin1String(buff));
        QStringList addtext = QStringList() << tmp;
        ui->comboBox_2->addItems(addtext);
    }
}

void windowmain::masg3()
{
    char buff[1024*100];
    memset(&buff,0,sizeof(buff));
    int sum = 0;
    int len = date.num;
    while (len) {
      ordsocket.waitForReadyRead(3);
      int ret = ordsocket.read(buff+sum,1024);
      len -= ret;
      sum += ret;
      if(ret == 0 )
          return;
      if(ret == -1)
          return;
    }
    qDebug() << sum;
    QPixmap pixmap;
    pixmap.loadFromData((const uchar *)buff, date.num, "JPEG");
    pixmap = pixmap.scaled(ui->label->width(),ui->label->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->label->setPixmap(pixmap);
}

void windowmain::closeEvent(QCloseEvent *event)
{
    ordsocket.close();
}
#endif
void windowmain::timeupdate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");//设置系统时间显示格式
    ui->label_2->setText(str);//在标签上显示时间
}




void windowmain::on_pushButton_clicked()
{

    ui->comboBox->blockSignals(true);
    //ui->comboBox_2->blockSignals(true);
   // Qstring a =combobox.selectvalue.tostring();

    ui->comboBox->clear();
    QString a = "选择日期";
    QStringList tmp = QStringList() << a;
    ui->comboBox->addItems(tmp);
    ui->comboBox_2->clear();
    QString b = "选择图像";
    QStringList tmp1 = QStringList() << b;
    ui->comboBox_2->addItems(tmp1);
    memset(&date,0,sizeof(date));
    date.mode = GETDATELIST;
    ordsocket.write((char *)&date,sizeof(date));
    ui->comboBox->blockSignals(false);
    //ui->comboBox_2->blockSignals(false);
#if 0
    int n  = 3;
    while(n--){
    QString a = "2017-04-28";
    QStringList tmp = QStringList() << a;
    ui->comboBox->addItems(tmp);
    }
#endif
}

void windowmain::conses()
{
    memset(&date,0,sizeof(date));
    date.mode = GETDATELIST;
    ordsocket.write((char *)&date,sizeof(date));
}



void windowmain::on_pushButton_2_clicked()
{
    memset(&date,0,sizeof(date));
    date.mode = DELETE;
    date.num = 0;
    QString a = ui->comboBox_2->currentText();
    QByteArray ba = a.toLatin1();
    strcpy(date.buff[0],ba.data());
    qDebug () << date.buff[0];
    ordsocket.write((char *)&date,sizeof(date));
}


void windowmain::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    QString ad = ui->comboBox_2->currentText();
    if(ad == "选择图像")
    {
        ui->label->clear();
        return;
    }
    memset(&date,0,sizeof(date));
    date.mode = GETPICTURE;
    date.num = 0;
    QString a = ui->comboBox_2->currentText();
    QByteArray ba = a.toLatin1();
    strcpy(date.buff[0],ba.data());
    ordsocket.write((char *)&date,sizeof(date));
}
int flag = 0;
void windowmain::on_pushButton_3_clicked()
{
    if(flag == 0)
    {
        flag = 1;
        ui->pushButton_3->setText("关闭采集");
        memset(&date,0,sizeof(date));
        date.mode = ON;
        ordsocket.write((char *)&date,sizeof(date));
        return;
    }
    flag = 0;
    ui->pushButton_3->setText("开启采集");
    memset(&date,0,sizeof(date));
    date.mode = OFF;
    ordsocket.write((char *)&date,sizeof(date));
}
