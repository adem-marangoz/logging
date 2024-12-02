/**
 * @file logging.cpp
 * @author adem marangoz (adem.marangoz95@gmail.com)
 * @brief This file contains the source code for the logging class.
 * @version 0.1
 * @date 2024-11-30
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "logging.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>


std::unique_ptr<logging> logging::instance = nullptr;
namespace logger {
    logging* log = logging::get_instance("app", true);
}
std::mutex logging::instance_mutex;


/**
 * @brief Constructs a new logging instance.
 * @param filename The filename to log to. If empty, logs to the console only.
 * @param print_log Whether to print the log to the console.
 *
 *If a filename is provided, the log file will be opened in append mode.
 *If the file cannot be opened, the logger will fall back to printing to the console
 *and a message will be printed to the standard error stream.
 *
 *The constructor also logs a message with the log level UNKNOWN to mark the start
 *of a new logger.
 */
logging::logging(std::string filename , bool print_log) :filename(filename) , print_log(print_log) 
{
    if(!filename.empty())
    {
        if(filename.find_last_of('.') == std::string::npos)
            filename += ".log";
        log_file.open(filename, std::ios::app);
        if (!log_file.is_open())
        {
            std::cerr << "Can't open log file " << filename << std::endl;
            print_log = false;
        }
    }

    add_log(log_level::UNKNOWN , "New logger" , "============================================");
}


/**
 * @brief Logs a message with a given log level, function name and message.
 *
 * The logged message will have the format "[time] [level] [function name] [message]". The time is the current time in the format
 * "%Y-%m-%d %H:%M:%S". The level is the string representation of the given log level. The function name is the given function name.
 * The message is the given message.
 *
 * If the logger is set to print the logs to the console, the logged message will be printed to the console. If the logger is set to log
 * to a file, the logged message will be appended to the file.
 */
void logging::add_log(log_level level, const char* fun_name, std::string message)
{

    std::string current_time = getCurrentTime();
    // [time] [level] [function name] [message]
    std::string log_message = "[" + formatField(current_time) + "] "
                            + "[" + formatField(logLevelToString(level)) + "] "
                            + "[" + formatField(fun_name) + "] "
                            + message + "\n";
    if(print_log)
        std::cout << log_message;

    if(log_file.is_open())
    {
        log_file.seekp(0, std::ios_base::end);
        log_file << log_message;
    }

}


/**
 * @brief Log a message with the default log level.
 * @param fun_name The name of the function that is logging.
 * @param message The message to log.
 * @details This function logs the message with the default log level.
 */
void logging::add_log(const char* fun_name, std::string message)
{
    std::string current_time = getCurrentTime();
    // [time] [level] [function name] [message]
    std::string log_message = "[" + formatField(current_time) + "] "
                            + "[" + formatField(logLevelToString(level)) + "] "
                            + "[" + formatField(fun_name) + "] "
                            + message + "\n";
    if(print_log)
        std::cout << log_message;

    if(log_file.is_open())
    {
        log_file.seekp(0, std::ios_base::end);
        log_file << log_message;
    }
}


/**
 * @brief Set the log level for the logger.
 * @param level The new log level.
 * @details This function sets the log level for the logger. All log messages
 *          with a level below the set log level will be discarded.
 */
void logging::set_log_level(log_level level)
{
     this->level = level; 
}


/**
 * @brief Get the global logging instance.
 * @param filename The filename to log to.
 * @param print_log Whether to print the log to the console.
 * @return The global logging instance.
 */
logging* logging::get_instance(std::string filename, bool print_log) {
    std::lock_guard<std::mutex> lock(instance_mutex); // Ensure thread safety
    if (!instance) {
        instance.reset(new logging(filename, print_log));
    }
    return instance.get();
}


/**
 * @brief Changes the log file used by the logger.
 * @param new_filename The new filename to log to.
 *
 * If a filename is provided, the log file will be opened in append mode.
 * If the file cannot be opened, the logger will fall back to printing to the console
 * and a message will be printed to the standard error stream. If the log file is
 * currently open, it will be closed before the new log file is opened.
 */
void logging::change_log_file(const std::string& new_filename)
{
    std::lock_guard<std::mutex> lock(instance_mutex); // Ensure thread safety

    // Close the current log file if it is open
    if (log_file.is_open())
    {
        log_file.close();
    }

    // Set the new filename and open the new log file
    filename = new_filename;
    if(!filename.empty())
    {
        if(filename.find_last_of('.') == std::string::npos)
            filename += ".log";
        log_file.open(filename, std::ios::app);
        if (!log_file.is_open())
        {
            std::cerr << "Can't open log file " << filename << std::endl;
            print_log = false;
        }
    }
}


/**
 * @brief Destructor for the logging class.
 *
 * Closes the log file if it is still open.
 */
logging::~logging()
{
    if(log_file.is_open())
        log_file.close();

}


/**
 * @brief Get the current time as a string in the format HH:MM:SS.SSS
 * 
 * @return std::string The current time
 */
std::string getCurrentTime() 
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);
    std::tm* time_info = std::localtime(&current_time);

    std::ostringstream oss;
    oss << std::put_time(time_info, "%H:%M:%S") << '.' 
        << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}


/**
 * @brief Format a string to a given width.
 *        If the string is longer than the width, it is cut to the width,
 *        otherwise it is padded with spaces to the width.
 * @param input The string to format.
 * @param width The width to format to.
 * @return The formatted string.
 */
std::string formatField(const std::string& input, size_t width) {
    if (input.length() > width) {
        return input.substr(0, width); // cut the string
    }
    return input + std::string(width - input.length(), ' '); // add spaces if shorter
}


/**
 * @brief Converts a log level to a string.
 * @param level The log level to convert.
 * @return A string representation of the log level.
 */
std::string logLevelToString(logging::log_level level) 
{
    switch (level) 
    {
        case logging::log_level::DEBUG:   return "DEBUG";
        case logging::log_level::INFO:    return "INFO";
        case logging::log_level::WARNING: return "WARNING";
        case logging::log_level::ERROR:   return "ERROR";
        case logging::log_level::FATAL:   return "FATAL";
        default:                      return "UNKNOWN";
    }
}