# Logging API

## Overview
This C++ logging API provides a simple and efficient way to log messages with various log levels. It supports logging to both console and a file, with thread-safe singleton access.

## Features
- Multiple log levels (DEBUG, INFO, WARNING, ERROR, FATAL, UNKNOWN)
- Singleton pattern for global access
- Thread-safe logging
- Option to log to a file or console
- Convenient log level constants

## Class: `logging`

### Enum: `log_level`
Defines the log levels:
- `DEBUG`
- `INFO`
- `WARNING`
- `ERROR`
- `FATAL`
- `UNKNOWN`

### Methods

#### Constructor
- `logging(std::string filename = "", bool print_log = true)`:
  - Constructs a logging instance with an optional filename and print log flag.

#### Destructor
- `~logging()`:
  - Destructs the logging instance, closing the log file if open.

#### Deleted Methods
- `logging() = delete`: Prevents default construction.
- `logging(const logging&) = delete`: Prevents copy construction.
- `logging(logging&&) = delete`: Prevents move construction.
- `logging& operator=(const logging&) = delete`: Prevents copy assignment.
- `logging& operator=(logging&&) = delete`: Prevents move assignment.

#### Static Methods
- `static logging* get_instance(std::string filename = "", bool print_log = true)`:
  - Returns a singleton instance of the logging class. Ensures thread safety and initializes the instance if it does not already exist.

- `friend std::string logLevelToString(log_level level)`:
  - Converts a log level to its string representation.

#### Log Level Methods
- `void set_log_level(log_level level)`:
  - Sets the log level for the logger. All log messages with a level below the set log level will be discarded.

- Constants for convenience:
  - `static constexpr log_level DEBUG = log_level::DEBUG`
  - `static constexpr log_level INFO = log_level::INFO`
  - `static constexpr log_level WARNING = log_level::WARNING`
  - `static constexpr log_level ERROR = log_level::ERROR`
  - `static constexpr log_level FATAL = log_level::FATAL`

#### Logging Methods
- `void add_log(log_level level, const char* fun_name, std::string message)`:
  - Logs a message with a specified log level, function name, and message.

- `void add_log(const char* fun_name, std::string message)`:
  - Logs a message with the default log level, function name, and message.

- `void change_log_file(const std::string& new_filename)`:
  - Changes the log file used by the logger.

## Usage Example

```cpp
#include "logging.hpp"
#include <thread>

void log_function() {
    logger::log->add_log(logging::log_level::INFO, __FUNCTION__, "Logging from thread");
}

void log_function1() {
    logger::log->set_log_level(logging::log_level::ERROR);
    logger::log->add_log(__FUNCTION__, "Logging from thread");
}

void log_function2() {
    logger::log->add_log(logging::log_level::INFO, __FUNCTION__, "Logging from thread");
}

int main() {
    // Get the logger instance
    logging* logger = logging::get_instance("app.log", true);
    
    // Set the log level
    logger->set_log_level(logging::log_level::DEBUG);
    
    // Add logs
    logger->add_log(logging::log_level::INFO, __FUNCTION__, "Application started");
    logger->add_log("main", "Default log level message");

    // Change the log file
    logger->change_log_file("new_app.log");
    
    // Create threads to log messages
    std::thread t1(log_function);
    std::thread t2(log_function1);
    std::thread t3(log_function2);

    t1.join();
    t2.join();
    t3.join();
    
    return 0;
}
```

## Example Output

```
[22:02:51.865   ] [UNKNOWN        ] [New logger     ] ============================================
[22:02:51.866   ] [ERROR          ] [log_function1  ] Logging from thread
[22:02:51.866   ] [INFO           ] [log_function   ] Logging from thread
[22:02:51.866   ] [INFO           ] [log_function2  ] Logging from thread
```

## Notes
- The `logging` class uses a singleton pattern to ensure that only one instance of the logger exists.
- Thread safety is ensured by using `std::mutex` for synchronizing access to the singleton instance.
- If the filename is provided, logs will be written to the file in append mode. If the file cannot be opened, logs will fall back to being printed to the console.


## Contributing
Contributions are welcome! Please open an issue or submit a pull request.

