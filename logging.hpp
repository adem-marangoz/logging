
/**
 * @file logging.hpp
 * @author adem marangoz (adem.marangoz95@gmail.com)
 * @brief This class provides a logging mechanism for the application. It allows
 *        to log messages with different log levels and to set the log level for
 *        the logger. The logger can also be configured to print the log messages
 *        to the standard output or to a file.
 * @version 0.1
 * @date 2024-11-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <mutex>

class logging
{
public:
    /* enum class log_level */
    enum class log_level: uint8_t
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL,
        UNKNOWN
    };

private:
    static std::unique_ptr<logging> instance;
    static std::mutex instance_mutex;
    bool print_log;
    std::string filename;
    std::ofstream log_file;
    log_level level;

    logging() = delete;
    logging(std::string filename = "" , bool print_log = true);
public:
    
    logging(const logging&) = delete;               // delete copy constructor
    logging(logging&&) = delete;                    // delete move constructor
    logging& operator=(const logging&) = delete;    // delete copy assignment
    logging& operator=(logging&&) = delete;         // delete move assignment
    ~logging();

    friend std::string logLevelToString(log_level level);

    static logging* get_instance(std::string filename = "", bool print_log = true);
    
    void add_log(log_level level, const char* fun_name, std::string message);
    void add_log(const char* fun_name, std::string message);
    void set_log_level(log_level level);
    void change_log_file(const std::string& new_filename);


    static constexpr log_level DEBUG = log_level::DEBUG;
    static constexpr log_level INFO = log_level::INFO;
    static constexpr log_level WARNING = log_level::WARNING;
    static constexpr log_level ERROR = log_level::ERROR;
    static constexpr log_level FATAL = log_level::FATAL;
};


std::string getCurrentTime();
std::string formatField(const std::string& input, size_t width = 15);
namespace logger {
    extern logging* log;
}

