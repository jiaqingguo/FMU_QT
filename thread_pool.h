#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <QObject>
#include <QMap>
#include <qdatetime.h>

#include "fmi4cpp/fmi4cpp.hpp"
#include "fmi4cpp/fmi2/cs_slave.hpp"

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;


class fum_thread;
class thread_pool : public QObject
{
    Q_OBJECT
public:
    explicit thread_pool(QObject *parent = nullptr);

public:
	static thread_pool* instance();					// 申请ThreadPool对象

	void setCylclyData(const int& cycleFmuNum, const int& cycleTime);
	void add_thread(fum_thread* pThread, const int& number);
	void start_thread(fum_thread* pThread);	// 开启线程
	void start_thread(const int& number);	// 开启线程
	void start_thread(const int& number, const std::vector<double>& vecInputValue);	// 开启线程
	void start_next_thread();
	void stop_all_thread();							// 停止所有线程

//	void stopASortOfVehicleType(int iVehicleType);
	void stop_thread(const int& number);

	void stop();

	void run_contral(const int &flags);

	int get_thread_size();

	void addFmi2Fmu(const  std::string& strFilePath);

	void setFmi2FmuCount(const int& fmuCount);

	QVector< fmi2::fmu*> m_vecFmi2Fmu;
	QVector< std::unique_ptr<cs_slave>> m_vecSlave;

signals:
	void signal_fmu_thread_finished(int tab, const  std::vector<double> vecOutputValue);
private slots:

	void slot_thread_finished();					// 释放线程

private:
	static thread_pool* m_pSelf;
	//QList<fum_thread*>			m_runningThread;
	QMap<int, fum_thread*>		m_mapThread;
	int m_next_thread_num = -1;
	int m_control_flags = 1;
	int m_cycleTime = 0;
	int m_cycleFmuNum = 0;// fmu类的数量;
	int m_runFinshNum = 0;

	QDateTime m_startTime;
	QDateTime m_endTime;
	//int m_fmi2FmuCount = -1;// fmu类的数量;
	
};

#endif // THREAD_POOL_H
