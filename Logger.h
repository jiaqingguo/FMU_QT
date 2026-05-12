#pragma once
#include <string>
#include <fstream>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

/**
 * @brief 异步文件日志类，替代原 Clog。
 *        使用内存队列 + 后台线程写盘，文件只在 open/close 时各操作一次。
 */
class Logger
{
public:
    Logger();
    ~Logger();

    // 创建日志文件，dir 为目录，suffix 为文件名后缀（如 " 1553"）
    void open(const std::string& dir, const std::string& suffix);
    void close();

    // 写一条日志（线程安全，非阻塞入队）
    void write(const std::string& msg);

private:
    void workerLoop();

    std::ofstream       m_file;
    std::queue<std::string> m_queue;
    std::mutex          m_mutex;
    std::condition_variable m_cv;
    std::thread         m_worker;
    std::atomic<bool>   m_running{ false };
};