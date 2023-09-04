#include "log.hpp"
#include "utils.hpp"

#include <chrono>
#include <cstdio>
#include <ctime>
#include <fmt/format.h>
#include <iostream>


namespace rpc
{
    static Logger* global_logger { nullptr };
    Logger* Logger::get_global_logger()
    {
        if (global_logger) return global_logger;
        global_logger = new Logger();
        return global_logger;
    }
    void Logger::log()
    {
        while (!m_buffer.empty()) 
        {
            std::string message = m_buffer.front();
            std::cout << message << std::endl;
            m_buffer.pop();
        }
    }
    std::string loglevel_to_string(LogLevel loglevel)
    {
        switch (loglevel)
        {
            case LogLevel::Debug: return "Debug";
            case LogLevel::Info: return "InFo";
            case LogLevel::Error: return "Error";
            default: return "UNKNOW";
        }
    }
    LogLevel LogEvent::get_log_level() { return m_log_level; } 
    std::string LogEvent::get_file_name() { return m_file_name; }
    std::string LogEvent::get_log()  // 得到日志，并且进行格式化
    {
        // 得到线程号和进程号
        m_pid = rpc::utils::get_pid();
        m_thread_pid = rpc::utils::get_thread_id();

        std::time_t time_point = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto now = *std::localtime(&time_point);
        
        m_file_name = __FILE_NAME__;
        m_file_line = __LINE__;

        // 最终结果大概是这样的形式:[INFO][2023年9月3日16时21分37秒][文件名:/home/lzc/test_c++/main.cpp 行号 10]
        std::string result = fmt::format("[{}][{}年{}月{}日{}时{}分{}秒][文件名:{},行号 {},进程:{},线程:{}]", loglevel_to_string(m_log_level), now.tm_year + 1900 , now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec,__FILE__, __LINE__,m_pid, m_thread_pid);
        return result;
    }
    void Logger::push_log(const std::string& message) 
    {
        m_buffer.emplace(message);
    }
}