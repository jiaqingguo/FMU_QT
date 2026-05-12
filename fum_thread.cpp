#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

#include <sstream>
#include <iomanip>
#include "fum_thread.h"

fum_thread::fum_thread()
{
    QString exeDir = QCoreApplication::applicationDirPath();
    QString logFilePath = exeDir + "//Log";

    m_logA.open(logFilePath.toStdString(), "fmu");
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

void fum_thread::set_fmu_file(const std::string& fmuPath, const double& stepSize, const int& calculateCount)
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

    m_pFmu = new fmi2::fmu(m_str_fmu_file_path);

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

// -----------------------------------------------------------------------
// 懒初始化 CSV 文件：首次写入时创建文件，并写入表头行
// 文件路径：<exe目录>/Log/output_tab<N>_thread<M>.csv
// 表头：index,val_0,val_1,...,val_K
// -----------------------------------------------------------------------
void fum_thread::ensure_csv_open()
{
    if (m_csvReady) return;

    QString exeDir = QCoreApplication::applicationDirPath();
    QString csvDir = exeDir + "/Log";
    QDir().mkpath(csvDir);  // 确保目录存在

    QString csvPath = QString("%1/output_tab%2_thread%3.csv")
        .arg(csvDir)
        .arg(m_cur_tab_index)
        .arg(m_thread_number);

    m_csvFile.open(csvPath.toStdString(), std::ios::out | std::ios::trunc);
    if (!m_csvFile.is_open()) return;

    // 写表头：index,val_0,val_1,...
    m_csvFile << "index";
    for (std::size_t i = 0; i < m_vecOutputValue.size(); ++i)
    {
        m_csvFile << ",val_" << i;
    }
    m_csvFile << "\n";
    m_csvFile.flush();

    m_csvReady = true;
}

// -----------------------------------------------------------------------
// 将本拍 m_vecOutputValue 追加一行到 CSV
// 格式：行号,val0,val1,...,valN
// -----------------------------------------------------------------------
void fum_thread::write_csv_row()
{
    ensure_csv_open();
    if (!m_csvFile.is_open()) return;

    // 行号 = 已完成拍数（m_calculateCount 已在 run() 中 -- 过，取反推）
    const int rowIndex = m_calculateCount;

    m_csvFile << rowIndex;
    for (const auto& val : m_vecOutputValue)
    {
        m_csvFile << "," << std::setprecision(10) << val;
    }
    m_csvFile << "\n";
    m_csvFile.flush();
}

void fum_thread::run()
{
    if (m_str_fmu_file_path.length() <= 0)
        return;
    if (m_thread_number < 0 || m_cur_tab_index < 0 || m_calculateCount <= 0)
        return;
    if (m_vecInputValue.size() <= 0 || m_vecInputValueReference.size() <= 0 || m_vecOutputValueReference.size() <= 0)
        return;

    if (m_sleepTime != 0)
    {
        sleep(m_sleepTime);
    }

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

    m_calculateCount--;

    // 将本拍输出数据写入 CSV 文件（一次一行，各值逗号分隔）
    write_csv_row();
}