#ifndef QTSERVERTHREADDEMO_H
#define QTSERVERTHREADDEMO_H

#include <QtWidgets/QWidget>
#include "ui_qtserverthreaddemo.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QFile>
#include <QString>
#include <qthread.h>
#pragma execution_character_set("utf-8")
QT_BEGIN_NAMESPACE
namespace Ui { class QtSeverThreadDemo; }
QT_END_NAMESPACE
class MyServer;
class SocketThread;

class QtServerThreadDemo : public QMainWindow
{
    Q_OBJECT

public:
    QtServerThreadDemo(QWidget *parent = 0);
    ~QtServerThreadDemo();

private:
    Ui::QtServerThreadDemoClass ui;
    QTcpServer *server;
    QTcpSocket *receivedSocket;
    QFile *newFile;
    QByteArray inBlock;
    QString fileName;
    qint64 totalSize;  //总共需要发送的文件大小（文件内容&文件名信息）
    qint64 byteReceived;  //已经发送的大小
    MyServer *tServer;
private slots:
    void acceptConnection();
    void readClient();
    void on_listenButton_clicked();
    void on_threadButton_clicked();
};
class MyServer :public QTcpServer{
    Q_OBJECT
public:
    qintptr socketDescriptor;
protected:
    virtual void incomingConnection(qintptr socketDescriptor);
};
class SocketThread :public QThread
{
    Q_OBJECT
public:
    //定义自己需要的方法或变量
    qintptr socketDescriptor;
    QTcpSocket * socket;//客户端的定义 void write_ptr(qintptr p){ ptr=p; } protected: virtual void run()
    void run();
private:
    QFile *newFile;
    QByteArray inBlock;
    QString fileName;
    qint64 totalSize;  //总共需要发送的文件大小（文件内容&文件名信息）
    qint64 byteReceived;  //已经发送的大小
private slots:
    void readyRead();
    void disconnected();
};
#endif // QTSERVERTHREADDEMO_H
