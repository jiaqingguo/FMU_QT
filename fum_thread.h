#ifndef FUM_THREAD_H
#define FUM_THREAD_H
#pragma once
#include <QThread>

//
#include "fmi4cpp/fmi4cpp.hpp"



class fum_thread : public QThread
{
    Q_OBJECT
public:
    fum_thread();

public:
    void set_thread_number(const int& number);
    int  get_thread_number();
    int  get_cur_tab();
  //  void set_step_size(double stepSize);
    void set_caculate_count_data(const int& cylce_count, const int tab_cout, const int& m_cur_tab_index);
    void set_cur_tab(const int& cur_tab_index);
    void set_fmu_file(const std::string& fmuPath, const double& stepSize);
    void set_input_value(const std::vector<fmi2Real>& vecInputValue);
    void set_input_reference(const std::vector<fmi2ValueReference>& vecInputValueReference);
    void set_input_data(const std::vector<fmi2Real>& vecInputValue, const std::vector<fmi2ValueReference>& vecInputValueReference);
    void set_output_reference(const std::vector<fmi2ValueReference>& vecOutputValueReference);
    std::vector<fmi2Real> get_output_data();
    void setSleepTime(const int& time);
private:
    void run();

    void use_fmu_caculate();
signals:
    void calucate_finsh();

private:
    bool m_bRun = true;
    int m_thread_number = -1;   // 当前线程数;
  //  int m_cur_cycle = -1;       // 当前循环;
    int m_cur_tab_index = -1;   // 对应tab页;
    double  m_stepSize = 1;
    int m_sleepTime = 0;

    // int m_tab_cout = 0; 
    QVector<int> m_vecAlgorithNum;
    std::string m_str_fmu_file_path;
    std::vector<fmi2ValueReference> m_vecInputValueReference;

    std::vector<fmi2ValueReference> m_vecOutputValueReference;
    std::vector<fmi2Real> m_vecInputValue;
    std::vector<fmi2Real> m_vecOutputValue;
};

#endif // FUM_THREAD_H
