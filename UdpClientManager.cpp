#include "UdpClientManager.h"

//#include <QThreadPool>
#include <QDebug>


UdpClientManager::UdpClientManager(QObject *parent)
	: QObject(parent)
	, m_udpSocket(0)
{

}

UdpClientManager::~UdpClientManager()
{
	if (m_udpSocket)
		m_udpSocket->deleteLater();
}

void UdpClientManager::startListen(const QString &ip, int port)
{
	m_udpSocket = new QUdpSocket(this);
	connect(m_udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));

	m_udpSocket->bind(QHostAddress(ip), port);
}

void UdpClientManager::startListen(int port)
{
	m_udpSocket = new QUdpSocket(this);
	connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

	bool ret= m_udpSocket->bind(QHostAddress::Any, port);
	if (ret)
	{
		qDebug() << "udp bind success!!!!";
	}
	else
	{
		qDebug() << "udp bind failed!!!!!!!!!!!!!!!!";

	}
}

void UdpClientManager::sendData(const QByteArray& datagram, const QHostAddress& host, quint16 port)
{
	m_udpSocket->writeDatagram(datagram, host, port);
	if (!m_bWarmedUp)
	{
		m_bWarmedUp = true;
		m_udpSocket->writeDatagram(datagram, host, port);
	}
}

int UdpClientManager::sendData(const QByteArray& datagram)
{
	if (m_targetPort != 0)
	{
		int Len = m_udpSocket->writeDatagram(datagram, m_targetAddress, m_targetPort);
		if (!m_bWarmedUp)
		{
			m_bWarmedUp = true;
			m_udpSocket->writeDatagram(datagram, m_targetAddress, m_targetPort);
		}
		qDebug() << "udp send len: " << Len;
		return Len;
	}
	return 0;
}

void UdpClientManager::setTarget(const QHostAddress& host, quint16 port)
{
	m_targetAddress = host;
	m_targetPort = port;
}

void UdpClientManager::processPendingDatagrams()
{
	while(m_udpSocket->hasPendingDatagrams())
	{
		QByteArray datagram;
		datagram.resize(m_udpSocket->pendingDatagramSize());
		//m_udpSocket->readDatagram(datagram.data(),datagram.size());
		QHostAddress srcAddress;
		quint16 srcPort;
		m_udpSocket->readDatagram(datagram.data(), datagram.size(), &srcAddress, &srcPort);

		//m_lastSenderAddress = srcAddress;
		//m_lastSenderPort = srcPort;

		qDebug() << "udp recv:" << srcAddress << srcPort;
		qDebug() << datagram;
		//sendData(datagram, srcAddress, srcPort);
		emit msgRecevied(datagram);

		//在子线程中处理数据
		//UdpDataTask *dataTask = new UdpDataTask(datagram,conditionList.last());
		//QThreadPool::globalInstance()->start(dataTask);
	}
}

