#include "logging.hpp"
#include <iostream>
#include <thread>


void log_function() {
    logger::log->add_log(logging::log_level::INFO, __FUNCTION__, "Logging from thread");
}
void log_function1() {
    logger::log->set_log_level(logging::log_level::ERROR);
    logger::log->add_log(__FUNCTION__, "Logging from thread1");
    logger::log->add_log(__FUNCTION__, "Logging from thread2");
    logger::log->add_log(__FUNCTION__, "Logging from thread3");
}
void log_function2() {
    logger::log->add_log(logging::log_level::INFO, __FUNCTION__, "Logging from thread");
}

int main() {
    // logger::log->change_log_file("new_app.log");
    std::thread t1(log_function);
    std::thread t2(log_function1);
    std::thread t3(log_function2);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}