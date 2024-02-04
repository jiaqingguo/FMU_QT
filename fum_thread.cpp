#include <QFileInfo>
#include <QDebug>

#include "fum_thread.h"



fum_thread::fum_thread()
{

}

void fum_thread::set_thread_number(const int& number)
{
    m_thread_number = number;
}

int fum_thread::get_thread_number()
{
    return m_thread_number;
}

int fum_thread::get_cur_tab()
{
    return m_cur_tab_index;
}

void fum_thread::set_caculate_count_data(const int& cylce_count, const int tab_cout, const int& m_cur_tab_index)
{
}

void fum_thread::set_cur_tab(const int& cur_tab_index)
{
    m_cur_tab_index = cur_tab_index;
}

void fum_thread::set_fmu_file(const std::string& fmuPath,const double &stepSize,const int& calculateCount)
{
    m_str_fmu_file_path = fmuPath;
    m_stepSize = stepSize;

    m_calculateCount = calculateCount;
    QFileInfo fileInfo(QString::fromStdString(m_str_fmu_file_path));
    if (!fileInfo.isFile())
    {
        qDebug() << QString::fromStdString(m_str_fmu_file_path) << " not exist!!!!";
        return;
    }
    if (m_pFmu != nullptr)
    {
        delete m_pFmu;
        m_pFmu = nullptr;
    }

    m_pFmu = new  fmi2::fmu(m_str_fmu_file_path);

    auto cs_fmu = m_pFmu->as_cs_fmu();
    auto md = cs_fmu->get_model_description();
    m_pSlvae = (cs_fmu->new_instance());
    m_pSlvae->setup_experiment();
    m_pSlvae->enter_initialization_mode();
    m_pSlvae->exit_initialization_mode();
}



void fum_thread::set_input_value(const std::vector<fmi2Real>& vecInputValue)
{
    m_vecInputValue = vecInputValue;
}

void fum_thread::set_input_reference(const std::vector<fmi2ValueReference>& vecInputValueReference)
{
    m_vecInputValueReference = vecInputValueReference;
    m_vecInputValue.resize(vecInputValueReference.size());
}

void fum_thread::set_input_data(const std::vector<fmi2Real>& vecInputValue, const std::vector<fmi2ValueReference>& vecInputValueReference)
{
    m_vecInputValue = vecInputValue;
    m_vecInputValueReference = vecInputValueReference;
}

void fum_thread::set_output_reference(const std::vector<fmi2ValueReference>& vecOutputValueReference)
{
    m_vecOutputValueReference = vecOutputValueReference;
    m_vecOutputValue.resize(m_vecOutputValueReference.size());
}

std::vector<fmi2Real> fum_thread::get_output_data()
{
    return m_vecOutputValue;
}

void fum_thread::setSleepTime(const int& time)
{
    m_sleepTime = time;
}

bool fum_thread::IsRunFinsh()
{
    return m_calculateCount == 0;
}

void fum_thread::run()
{
   if (m_str_fmu_file_path.length() <= 0)
        return;
    if (m_thread_number < 0 || m_cur_tab_index <0 || m_calculateCount <=0)
        return;
    if (m_vecInputValue.size() <= 0 || m_vecInputValueReference.size() <= 0 || m_vecOutputValueReference.size() <= 0)
        return;

    if (m_sleepTime != 0)
    {
        sleep(m_sleepTime);
    }
    
    //while (true)
    //{
    //    if (m_bRun && m_calculateCount>=0)
    //    {
            if (!m_pSlvae->write_real(m_vecInputValueReference, m_vecInputValue))
            {
                //return;
            }

            if (!m_pSlvae->step(m_stepSize))
            {
                //return;
            }

            if (!m_pSlvae->read_real(m_vecOutputValueReference, m_vecOutputValue))
            {
                //return;
            }
           // m_bRun = false;
            m_calculateCount--;
        
       /* if (m_calculateCount == 0)
        {
            
        }*/
     
}
