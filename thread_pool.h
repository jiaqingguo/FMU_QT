#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <QObject>
#include <QMap>



class fum_thread;
class thread_pool : public QObject
{
    Q_OBJECT
public:
    explicit thread_pool(QObject *parent = nullptr);

public:
	static thread_pool* instance();					// ����ThreadPool����

	void add_thread(fum_thread* pThread, const int& number);
	void start_thread(fum_thread* pThread);	// �����߳�
	void start_thread(const int& number);	// �����߳�
	void start_thread(const int& number, const std::vector<double>& vecInputValue);	// �����߳�

	void stop_all_thread();							// ֹͣ�����߳�

//	void stopASortOfVehicleType(int iVehicleType);
	void stop_thread(const int& number);

	void stop();

private slots:

	void slot_thread_finished();					// �ͷ��߳�

private:
	static thread_pool* m_pSelf;
	//QList<fum_thread*>			m_runningThread;
	QMap<int, fum_thread*>		m_mapThread;

};

#endif // THREAD_POOL_H
