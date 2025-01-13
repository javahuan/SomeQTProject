#include "qtserverthreaddemo.h"
#include <QTextCodec>
QtServerThreadDemo::QtServerThreadDemo(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
}

QtServerThreadDemo::~QtServerThreadDemo()
{

}
void QtServerThreadDemo::acceptConnection(){
    receivedSocket = server->nextPendingConnection();
    ui.textEdit->append(QStringLiteral("有连接上来了！"));
    connect(receivedSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
}
void QtServerThreadDemo::readClient(){
    if(byteReceived == 0)  //才刚开始接收数据，此数据为文件信息
    {
        QDataStream in(receivedSocket);
        in>>totalSize>>byteReceived>>fileName;
        fileName = "D://Recv//" + fileName;
        newFile = new QFile(fileName);
        newFile->open(QFile::WriteOnly);
    }else{
        inBlock = receivedSocket->readAll();
        byteReceived += inBlock.size();
        newFile->write(inBlock);
        newFile->flush();
    }
    if(byteReceived == totalSize){
        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
    }

}
void QtServerThreadDemo::on_listenButton_clicked(){
    totalSize = 0;
    byteReceived = 0;
    server = new QTcpServer(this);
    server->listen(QHostAddress("192.168.2.111"), 8888);
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    ui.textEdit->append(QStringLiteral("开始监听..."));
}
void MyServer::incomingConnection(qintptr socketDescriptor){
    qDebug()<<"New Connect is connect"<<socketDescriptor;
    SocketThread * thread=new SocketThread();
    thread->socketDescriptor=socketDescriptor;
    thread->start();
}

void SocketThread::run(){
    byteReceived=0;
    totalSize=0;
    socket=new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);//客户端的初始化
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    exec();
}
void QtServerThreadDemo::on_threadButton_clicked(){
    tServer=new MyServer();
    tServer->listen(QHostAddress::AnyIPv4,8888);
}
void SocketThread::readyRead(){
    if(byteReceived == 0)  //才刚开始接收数据，此数据为文件信息
    {
        QDataStream in(socket);
        in>>totalSize>>byteReceived>>fileName;
        fileName = "D://Recv//" + fileName;
        newFile = new QFile(fileName);
        newFile->open(QFile::WriteOnly);
    }else{
        inBlock = socket->readAll();
        byteReceived += inBlock.size();
        newFile->write(inBlock);
        newFile->flush();
    }
    if(byteReceived == totalSize){
        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
    }
}
void SocketThread::disconnected(){
    newFile->close();
    socket->deleteLater();
    exit(0);
}
