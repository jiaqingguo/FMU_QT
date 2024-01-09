#pragma execution_character_set("utf-8")

#include "appkey.h"
#include "qmutex.h"
#include "qfile.h"
#include "qtimer.h"
#include "qdatetime.h"
#include "qapplication.h"
#include "qmessagebox.h"
#include "qdebug.h"
#include "qprocess.h"

AppKey *AppKey::self = NULL;
AppKey *AppKey::Instance()
{
    if (!self) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!self) {
            self = new AppKey;
        }
    }

    return self;
}

AppKey::AppKey(QObject *parent) : QObject(parent)
{
    keyData = "";
    keyUseDate = false;
    keyDate = "2017-01-01";
    keyUseRun = false;
    keyRun = 1;
    keyUseCount = false;
    keyCount = 10;

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkTime()));
    startTime = QDateTime::currentDateTime();
}

void AppKey::start()
{
    //判断密钥文件是否存在,不存在则从资源文件复制出来,同时需要设置文件写权限
    QString keyName = qApp->applicationDirPath() + "/licence.txt";
    QFile keyFile(keyName);
    if (!keyFile.exists() || keyFile.size() == 0)
    {
        qDebug()<<keyName;
        QMessageBox::critical(0, "错误", "密钥文件丢失,请联系供应商!");
        exit(0);
    }

    //读取密钥文件
    keyFile.open(QFile::ReadOnly);
    keyData = keyFile.readLine();
    keyFile.close();

    //将从licence文件中的密文解密,与当前时间比较是否到期
    keyData = getXorEncryptDecrypt(keyData, 100);
    qDebug()<<"keyData:  "<<keyData;
    QStringList data = keyData.split("|");

    if (data.count() != 10)
    {
        QMessageBox::critical(0, "错误", "licence文件已损坏,程序将自动关闭!");
        exit(0);
    }

    keyUseDate = (data.at(0) == "1");
    keyDate = data.at(1);

    keyUselastOpen = (data.at(2) == "1");
    keylastOpen = data.at(3);

    keyUseUUID = (data.at(4) == "1");
    keyUUID = data.at(5);

    keyUseRun = (data.at(6) == "1");
    keyRun = data.at(7).toInt();
    keyUseCount = (data.at(8) == "1");
    keyCount = data.at(9).toInt();

    //如果启用了时间限制
    if (keyUseDate)
    {
        QString nowDate = QDate::currentDate().toString("yyyy-MM-dd");
        if (nowDate > keyDate)
        {
            QMessageBox::critical(0, "错误", "软件已到期,请联系供应商更新licence!");
            exit(0);
        }
    }

    if(true)
    {
        qDebug()<<"lastopne:"<<keylastOpen;
        QString nowDate = QDate::currentDate().toString("yyyy-MM-dd hh:mm:ss");
        if (nowDate<keyUselastOpen)
        {
            QMessageBox::critical(0, "错误", "软件已到期,请联系供应商更新licence!");
            exit(0);
        }
    }

     qDebug()<<"uuid:"<<keyUUID;
     if (keyUseUUID)
     {
         if (keyUUID != getComputerUUID())
         {
             QMessageBox::critical(0, "错误", "电脑唯一标识错误,请联系供应商更新licence!");
             exit(0);
         }
     }

    //如果启用了运行时间显示
    if (keyUseRun)
    {
        timer->start();
    }

    updateLicenseTxt();
}

void AppKey::stop()
{
    timer->stop();
}

void AppKey::checkTime()
{
    //找出当前时间与首次启动时间比较
    QDateTime now = QDateTime::currentDateTime();
    if (startTime.secsTo(now) >= (keyRun * 60))
    {
        QMessageBox::critical(0, "错误", "试运行时间已到,请联系供应商更新licence!");
        exit(0);
    }
}

QString AppKey::getXorEncryptDecrypt(const QString &data, char key)
{
    //采用异或加密,也可以自行更改算法
    QByteArray buffer = data.toLatin1();
    int size = buffer.size();
    for (int i = 0; i < size; i++)
    {
        buffer[i] = buffer.at(i) ^ key;
    }

    return QLatin1String(buffer);
}

bool AppKey::checkCount(int count)
{
    if (keyUseCount)
    {
        if (count >= keyCount)
        {
            QMessageBox::critical(0, "错误", "设备数量超过限制,请联系供应商更新licence!");
            return false;
        }
    }

    return true;
}

void AppKey::updateLicenseTxt()
{
    keylastOpen= QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss");
    QString key = QString("%1|%2|%3|%4|%5|%6|%7|%8|%9|%10").arg(keyUseDate).arg(keyDate).arg(keyUselastOpen).arg(keylastOpen)
            .arg(keyUseUUID).arg(keyUUID).arg(keyUseRun).arg(keyRun).arg(keyUseCount).arg(keyCount);

    QFile file(QApplication::applicationDirPath() + "/licence.txt");
    file.open(QFile::WriteOnly | QIODevice::Text);
    file.write(getXorEncryptDecrypt(key, 100).toLatin1());
    file.close();
   //QMessageBox::critical(0, "提示", "更新密钥成功,打开时间已修改!");
}

QString AppKey::getComputerUUID()
{

    return getWMIC("wmic csproduct get uuid");

}

QString AppKey::getWMIC(const QString &cmd)
{
    //获取cpu名称：wmic cpu get Name
    //获取cpu核心数：wmic cpu get NumberOfCores
    //获取cpu线程数：wmic cpu get NumberOfLogicalProcessors
    //查询cpu序列号：wmic cpu get processorid
    //查询主板序列号：wmic baseboard get serialnumber
    //查询BIOS序列号：wmic bios get serialnumber
    //查看硬盘：wmic diskdrive get serialnumber
    QProcess p;
    p.start(cmd);
    p.waitForFinished();
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    QStringList list = cmd.split(" ");
    result = result.remove(list.last(), Qt::CaseInsensitive);
    result = result.replace("\r", "");
    result = result.replace("\n", "");
    result = result.simplified();
    return result;
}

