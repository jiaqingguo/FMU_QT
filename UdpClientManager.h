#ifndef UDPCLIENTMANAGER_H
#define UDPCLIENTMANAGER_H

#include <QObject>
#include <QUdpSocket>

//#include "network_global.h"


class  UdpClientManager : public QObject
{
	Q_OBJECT

public:
	UdpClientManager(QObject *parent = 0);
	~UdpClientManager();

	void startListen(const QString &ip,int port);
	void startListen(int port);

	void sendData(const QByteArray& datagram, const QHostAddress& host, quint16 port);
	void sendData(const QByteArray& datagram);
private:
	//
	QUdpSocket* m_udpSocket;
	QHostAddress m_lastSenderAddress;
	quint16 m_lastSenderPort = 0;

private slots:
	//接收服务器传来的数据
	void processPendingDatagrams();

signals:
	void msgRecevied(const QByteArray &);
};


#endif // ENGIUDPCLIENTMANAGER_H
