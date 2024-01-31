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

void thread_pool::setCylclyData(const int &cycleFmuNum, const int& cycleTime)
{
	m_cycleFmuNum = cycleFmuNum;
	m_cycleTime = cycleTime;
	m_runFinshNum = 0;
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
	m_control_flags = 1;
	if (m_mapThread.contains(number))
	{
		auto& pThread = m_mapThread[number];
		pThread->set_input_value(vecInputValue);
		pThread->start();
	}
}

void thread_pool::start_next_thread()
{
	if (m_mapThread.contains(m_next_thread_num))
	{
		fum_thread* pThread_next = m_mapThread[m_next_thread_num];
		int tab = pThread_next->get_cur_tab();
		// 2.1 �������;
		auto vecInputValue = g_pWidget->get_algorithm_tableWidget_input(tab);
		pThread_next->set_input_value(vecInputValue);

		if (m_runFinshNum ==m_cycleFmuNum  && m_cycleTime !=0)
		{
			m_runFinshNum = 0;
			pThread_next->setSleepTime(m_cycleTime);
		}
		pThread_next->start();
	}
	else
	{
		m_runFinshNum = 0;
	}
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
	for (auto itor = m_mapThread.begin(); itor != m_mapThread.end(); )
	{
		fum_thread* pThread = itor.value();
		if (pThread)
		{
			if (pThread->isRunning())
			{
				pThread->wait();
			}
			//int key = itor.key();
			itor= m_mapThread.erase(itor);
			

			pThread->deleteLater();
		}
	}
}

void thread_pool::run_contral(const int& flags)
{
	
	if (flags == 1 || flags==3) // ����
	{
		m_control_flags = 1;
		start_next_thread();
	}
	else if (flags == 0) //ֹͣ;
	{
		m_control_flags = flags;
		stop();
	}
	else if (flags == 2)// �������ͣ �����������ж�;
	{
		m_control_flags = flags;
	}

}

int thread_pool::get_thread_size()
{
	return m_mapThread.size();
}


void thread_pool::slot_thread_finished()
{
	fum_thread * pThread = static_cast<fum_thread*>(sender());
	if (pThread)
	{
		m_runFinshNum++;
		// 1.����߳�ִ�н�� ˢ��������;
		// 1.1 ˢ�½��
		auto vecOutputData =pThread->get_output_data();
		
		int tab = pThread->get_cur_tab();
		
		g_pWidget->update_algorithhnum_opersition(tab, vecOutputData);
		//emit signal_fmu_thread_finished(tab, vecOutputData);
		// 1.2 ���ݼ�������


		int number = pThread->get_thread_number();

		m_next_thread_num = pThread->get_thread_number() + 1;
		// �Ƴ�ִ������߳�;
		m_mapThread.remove(pThread->get_thread_number());
		//pThread->deleteLater();
		pThread->wait();
		pThread->deleteLater();

		// 1.2ˢ�� �����˿�;
		// 
		// 


		// ȫ��ִ����� ���ÿ��ư�ť״̬;
		if (m_mapThread.size() == 0)
		{
			g_pWidget->reset_control_btns();
		}
		// 2.������һ���߳�;
		if (m_control_flags==1)
		{
			start_next_thread();
		}



	/*	else if(m_control_flags==)
		{

		}*/

		//if (m_mapThread.contains(m_next_thread_num))
		//{
		//	fum_thread* pThread_next = m_mapThread[m_next_thread_num];
		//	int tab = pThread_next->get_cur_tab();
		//	// 2.1 �������;
		//	auto vecInputValue = g_pWidget->get_algorithm_tableWidget_input(tab);
		//	pThread_next->set_input_value(vecInputValue);

		//	pThread_next->start();
		//}
	}
}