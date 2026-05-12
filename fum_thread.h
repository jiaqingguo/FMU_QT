#ifndef FUM_THREAD_H
#define FUM_THREAD_H
#pragma once
#include <QThread>
#include <fstream>

#include "fmi4cpp/fmi4cpp.hpp"
#include "fmi4cpp/fmi2/cs_slave.hpp"
#include "Logger.h"

using namespace fmi4cpp;
using namespace fmi4cpp::fmi2;

class fum_thread : public QThread
{
    Q_OBJECT
public:
    fum_thread();

public:
    void set_thread_number(const int& number);
    int  get_thread_number();
    int  get_cur_tab();
    void set_caculate_count_data(const int& cylce_count, const int tab_cout, const int& m_cur_tab_index);
    void set_cur_tab(const int& cur_tab_index);
    void set_fmu_file(const std::string& fmuPath, const double& stepSize, const int& calculateCount);

    void set_input_value(const std::vector<fmi2Real>& vecInputValue);
    void set_input_reference(const std::vector<fmi2ValueReference>& vecInputValueReference);
    void set_input_data(const std::vector<fmi2Real>& vecInputValue, const std::vector<fmi2ValueReference>& vecInputValueReference);
    void set_output_reference(const std::vector<fmi2ValueReference>& vecOutputValueReference);
    std::vector<fmi2Real> get_output_data();
    void setSleepTime(const int& time);

    bool IsRunFinsh();

private:
    void run();
    void write_csv_row();           // 追加一行输出数据至 CSV
    void ensure_csv_open();         // 懒初始化：首次调用时创建 CSV 文件并写表头

signals:
    void calucate_finsh();

private:
    bool m_bRun = false;
    int m_thread_number = -1;
    int m_calculateCount = -1;
    int m_cur_tab_index = -1;
    double  m_stepSize = 1;
    int m_sleepTime = 0;

    QVector<int> m_vecAlgorithNum;
    std::string m_str_fmu_file_path;
    std::vector<fmi2ValueReference> m_vecInputValueReference;
    std::vector<fmi2ValueReference> m_vecOutputValueReference;
    std::vector<fmi2Real> m_vecInputValue;
    std::vector<fmi2Real> m_vecOutputValue;

    fmi2::fmu* m_pFmu = nullptr;
    std::unique_ptr<cs_slave> m_pSlvae;

    Logger          m_logA;
    std::ofstream   m_csvFile;          // CSV 输出文件流
    bool            m_csvReady = false; // 是否已写入表头
};

#endif // FUM_THREAD_H