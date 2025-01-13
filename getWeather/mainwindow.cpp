#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlError>
#include<QSqlTableModel>
#include<QSqlQuery>
#include<QtCore>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);  //新建QNetworkAccessManager对象
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));//关联信号和槽
    connectDB();
    //另起一个timer，显示秒表
    QTimer *timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));//关联信号和槽
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString local_city = ui->city->text().trimmed(); //获得需要查询天气的城市名称
    char quest_array[256]="http://www.weather.com.cn/data/sk/101010100.html?city=";
    QNetworkRequest quest;
    sprintf(quest_array,"%s%s",quest_array,local_city.toUtf8().data());
    qDebug()<<"get URL:"<<quest_array;
    quest.setUrl(QUrl(quest_array));
    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    /*发送get网络请求*/
    manager->get(quest);
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::replyFinished(QNetworkReply *reply)
{

    qDebug()<<"recv weather data!!";
    QString all = reply->readAll();
    ui->resultEdit->setText(all); //将接收到的数据显示出来

    QJsonParseError err;
    QJsonDocument json_recv = QJsonDocument::fromJson(all.toUtf8(),&err);//解析json对象
    qDebug() <<"[json]"<< err.error;
    if(!json_recv.isNull())
    {
        qDebug()<<"进了第一个if";
        QJsonObject object = json_recv.object();

        if(object.contains("weatherinfo"))
        {
            qDebug()<<"进了第二个if";
            QJsonValue value = object.value("weatherinfo");  // 获取指定 key 对应的 value
//            if(value.isObject())
//            {
                qDebug()<<"进了第三个if";
                QJsonObject object_data = value.toObject();
                qDebug()<<object_data;
//                if(object_data.contains("object"))
//                {
//                    qDebug()<<"进了第四个if";
//                    //QJsonValue value = object_data.value("object");
//                    value=value.toArray();
//                    qDebug()<<"value.isArray()?  "<<value;
//                    if(value.isArray())
//                    {
                        qDebug()<<"准备获取温度等信息";
                       // QJsonObject today_weather = value.toArray().at(0).toObject();
                        QJsonObject today_weather = object_data;

                        weather_type = today_weather.value("SD").toString();
//                        QString low = today_weather.value("low").toString();
//                        QString high = today_weather.value("high").toString();
//                        wendu = low.mid(low.length()-3,4) +"~"+ high.mid(high.length()-3,4);
                        wendu=today_weather.value("temp").toString();
                        QString strength = today_weather.value("fengli").toString();
                        strength.remove(0,8);
                        strength.remove(strength.length()-2,2);
                        fengli = today_weather.value("WD").toString() + strength;

                        ui->type->setText(weather_type); //显示天气类型
                        ui->wendu->setText(wendu);   //显示温度
                        ui->fengli->setText(fengli); //显示风力
                   // }
               // }
           // }
        }

    }else
    {
        qDebug()<<"json_recv is NULL or is not a object !!";
    }
    reply->deleteLater(); //销毁请求对象

}

void MainWindow::connectDB()
{
    db =QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");//连接本地主机
    db.setPort(3306);
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("123456");

    bool ok = db.open();
    if(ok){
        qDebug()<<"数据库连接成功";
    }
    else{
        qDebug()<<"数据库连接失败";
        qDebug()<<"error open database because"<<db.lastError().text();
    }
}

void MainWindow::on_showAllButton_clicked()
{
    model=new QSqlTableModel(this);
    model->setTable("weather");
    model->select();
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);
    model->setSort(0,Qt::DescendingOrder);
    model->select();
}

void MainWindow::on_SaveallButton_clicked()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QSqlQuery query(db);
    QString city=ui->city->text();
    QString temp=ui->wendu->text();
    QString humidity=ui->type->text();
    QString wind=ui->fengli->text();
    QString userId="666";
    QString userName="大头";
    QString createTime=currentTime.toString("yyyy-MM-dd hh:mm:ss");
    QString sql="insert into weather(city,temperature,humidity,wind,creator_id,creator,create_time) values(";
    sql=sql+" '"+city+"','"+temp+"','"+humidity+"','"+wind+"','"+userId+"','"+userName+"','"+createTime+"') ";
    ui->sqlEdit->setText(sql);
    qDebug()<<"sql语句是："<<sql;
    query.exec(sql);


}

void MainWindow::on_timeButton_clicked()
{
    QTimer *timerGet=new QTimer(this);
    connect(timerGet,SIGNAL(timeout()),this,SLOT(timerGetWeather()));//关联信号和槽
    timerGet->start(10000);
}

void MainWindow::timerGetWeather()
{
    on_pushButton_clicked();
    on_SaveallButton_clicked();
    on_showAllButton_clicked();
}

void MainWindow::timerUpDate()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString str=currentTime.toString("yyyy-MM-dd hh:mm:ss");

    ui->timeLabel->setText(str);
}
