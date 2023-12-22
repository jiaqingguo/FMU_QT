#include "fum_thread.h"

using namespace fmi4cpp;

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

void fum_thread::set_fmu_file(const std::string& fmuPath)
{
    m_str_fmu_file_path = fmuPath;
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

//std::vector<fmi2Real> get_output_data()
//{
//   // return m_vecOutputValue;
//}

void fum_thread::run()
{
    if (m_str_fmu_file_path.length() <= 0)
        return;
    if (m_thread_number < 0 /*||  m_cur_cycle<0*/ || m_cur_tab_index <0)
        return;
    if (m_vecInputValue.size() <= 0 || m_vecInputValueReference.size() <= 0 || m_vecOutputValueReference.size() <= 0)
        return;

    // �����㷨;
    //std::string exe_config_paths = "D:\\CS\\atspaceMicroTimer.fmu";//m_fileInfo.filePath().toStdString();
    //std::string exe_config_paths = m_fileInfo.absoluteFilePath().toStdString();
    //const std::string fmu_path = string_To_UTF8(exe_config_paths);
  /*  while (m_bRun)
    {*/
        fmi2::fmu fmu(m_str_fmu_file_path);
        auto cs_fmu = fmu.as_cs_fmu();
        auto md = cs_fmu->get_model_description();
        auto slave1 = cs_fmu->new_instance();
        slave1->setup_experiment();
        slave1->enter_initialization_mode();
        slave1->exit_initialization_mode();


        //std::vector<fmi2Real> wef(m_vecInputValueReference.size());
        //std::vector<fmi2Real> ref(m_vecOutputValueReference.size());
        m_vecOutputValue.resize(m_vecOutputValueReference.size());

        m_vecOutputValue.clear();

        if (!slave1->write_real(m_vecInputValueReference, m_vecInputValue))
        {
            //return;
        }

        if (!slave1->step(1))
        {
            //return;
        }

        if (!slave1->read_real(m_vecOutputValueReference, m_vecOutputValue))
        {
            //return;
        }
          
  /*      m_bRun = false;
    }
   */

}

void fum_thread::use_fmu_caculate()
{


}
