#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDialog>
#include<QSqlDatabase>
#include<QSqlTableModel>

class QSqlTableModel;
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_exitButton_clicked();

    void replyFinished(QNetworkReply *reply);

    void on_showAllButton_clicked();

    void on_SaveallButton_clicked();

    void on_timeButton_clicked();
    void timerGetWeather();
    void timerUpDate();

private:
    QSqlTableModel *model;
    QSqlDatabase db;
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;  //请求句柄
    QString fengli;       //风力
    QString wendu;        //温度
    QString weather_type;  //天气类型
    void connectDB();
};
#endif // MAINWINDOW_H
