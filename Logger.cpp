#include "Logger.h"
#include <filesystem>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

Logger::Logger() = default;

Logger::~Logger()
{
    close();
}

void Logger::open(const std::string& dir, const std::string& suffix)
{
    if (m_running) return;

    // 确保目录存在
    fs::create_directories(dir);

    // 生成带时间戳的文件名
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_s(&tm, &t);
    std::ostringstream ss;
    ss << dir << "//" << std::put_time(&tm, "%Y%m%d_%H%M%S") << suffix << ".log";

    m_file.open(ss.str(), std::ios::app);
    if (!m_file.is_open()) return;

    m_running = true;
    m_worker = std::thread(&Logger::workerLoop, this);
}

void Logger::close()
{
    if (!m_running) return;
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_running = false;
    }
    m_cv.notify_all();
    if (m_worker.joinable()) m_worker.join();
    m_file.close();
}

void Logger::write(const std::string& msg)
{
    if (!m_running) return;
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_queue.push(msg + "\n");
    }
    m_cv.notify_one();
}

void Logger::workerLoop()
{
    while (true)
    {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this] { return !m_queue.empty() || !m_running; });

        // 批量刷出所有待写条目
        while (!m_queue.empty())
        {
            m_file << m_queue.front();
            m_queue.pop();
        }
        m_file.flush();

        if (!m_running && m_queue.empty()) break;
    }
}