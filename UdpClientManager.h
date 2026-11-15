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
	int sendData(const QByteArray& datagram);

	void setTarget(const QHostAddress& host, quint16 port);

	// 设置 DataServer 目标，供发送 JSON 协议数据使用
	void setDataServerTarget(const QHostAddress& host, quint16 port);

	// 发送数据到 DataServer（JSON 协议：fmu_selected / fmu_calculate）
	int sendToDataServer(const QByteArray& datagram);
private:
	//
	QUdpSocket* m_udpSocket;
	QHostAddress m_targetAddress;           // 遥控软件目标地址
	quint16 m_targetPort = 0;               // 遥控软件目标端口
	QHostAddress m_dataServerAddress;       // DataServer 目标地址
	quint16 m_dataServerPort = 0;           // DataServer 目标端口
	bool m_bWarmedUp = false;

private slots:
	//接收服务器传来的数据
	void processPendingDatagrams();

signals:
	void msgRecevied(const QByteArray &);
};


#endif // ENGIUDPCLIENTMANAGER_H
