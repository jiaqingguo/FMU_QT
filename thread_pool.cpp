#include <QDebug>
#include "thread_pool.h"
#include "fum_thread.h"
#include "widget.h"

thread_pool* thread_pool::m_pSelf = NULL;

thread_pool::thread_pool(QObject *parent) : QObject(parent)
{

}

thread_pool* thread_pool::instance()
{
	if (NULL == m_pSelf)
	{
		m_pSelf = new thread_pool();
	}

	return m_pSelf;
}

void thread_pool::add_thread(fum_thread* pThread, const int& number)
{
	if (NULL != pThread)
	{
		connect(pThread, SIGNAL(finished()), this, SLOT(slot_thread_finished()));
		m_mapThread.insert(number,pThread);
		
	}
}

void thread_pool::start_thread(fum_thread* pThread)
{
	if (NULL != pThread)
	{
		connect(pThread, SIGNAL(finished()), this, SLOT(slot_thread_finished()));
		//pThread->start();
	}
}

void thread_pool::start_thread(const int& number)
{
	if (m_mapThread.contains(number))
	{
		auto& pThread = m_mapThread[number];
		pThread->start();
	}
}

void thread_pool::start_thread(const int& number, const std::vector<double>& vecInputValue)
{
	if (m_mapThread.contains(number))
	{
		auto& pThread = m_mapThread[number];
		pThread->set_input_value(vecInputValue);
		pThread->start();
	}
}

void thread_pool::start_next_thread(const int& finsh_num)
{
	m_mapThread.remove(finsh_num);
	//pThread->deleteLater();
	
	// 2.������һ���߳�;

	//int next_number = finsh_num + 1;
	//if (m_mapThread.contains(next_number))
	//{
	//	fum_thread* pThread_next = m_mapThread[next_number];
	//	int tab = pThread_next->get_cur_tab();
	//	// 2.1 �������;
	//	//auto vecInputValue = g_pWidget->get_algorithm_tableWidget_input(tab);
	//	//pThread_next->set_input_value(vecInputValue);

	//	pThread_next->start();
	//}
}



void thread_pool::stop_all_thread()
{
	//�����̵߳�stop������Ȼ���̻߳�run�֮꣬�����slot_thread_finished�ۺ������ͷ���Դ���ڴ˺���
	for (auto itor = m_mapThread.begin(); itor != m_mapThread.end(); ++itor)
	{
		fum_thread* pThread = itor.value();
		if (pThread)
		{
			if (pThread->isRunning())
			{
				//pThread->stop();
			}
		}
	}
}



void thread_pool::stop_thread(const int& number)
{

}

void thread_pool::stop()
{
	for (auto itor = m_mapThread.begin(); itor != m_mapThread.end(); ++itor)
	{
		fum_thread* pThread = itor.value();
		if (pThread)
		{
			if (pThread->isRunning())
			{
				pThread->quit();
				pThread->wait();
				
			}

			m_mapThread.remove(pThread->get_thread_number());

			// 1.����߳�ִ�н�� ˢ��������;
			// 1.1 ˢ��������;
			
			// 1.2ˢ�� �����˿�;


			pThread->deleteLater();
		}
	}
}


void thread_pool::slot_thread_finished()
{
	fum_thread * pThread = static_cast<fum_thread*>(sender());
	if (pThread)
	{
		

		// 1.����߳�ִ�н�� ˢ��������;
		// 1.1 ˢ�½��
		auto vecOutputData =pThread->get_output_data();
		
		int tab = pThread->get_cur_tab();
		
		g_pWidget->update_algorithm_tableWidget_out(tab, vecOutputData);
		//emit signal_fmu_thread_finished(tab, vecOutputData);
		


		qDebug() << "thread " << pThread->get_thread_number() << "end------!";


		int number = pThread->get_thread_number();

		int next_number = pThread->get_thread_number() + 1;
		// �Ƴ�ִ������߳�;
		m_mapThread.remove(pThread->get_thread_number());
		//pThread->deleteLater();
		pThread->wait();
		pThread->deleteLater();

		// 1.2ˢ�� �����˿�;
		// 
		// 

		// 2.������һ���߳�;
		

		if (m_mapThread.contains(next_number))
		{
			fum_thread* pThread_next = m_mapThread[next_number];
			int tab = pThread_next->get_cur_tab();
			// 2.1 �������;
			auto vecInputValue = g_pWidget->get_algorithm_tableWidget_input(tab);
			pThread_next->set_input_value(vecInputValue);

			pThread_next->start();
		}
	}
}