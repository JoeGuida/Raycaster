#ifndef LOGGER_H
#define LOGGER_H

#include <cstdlib>
#include <chrono>
#include <iostream>
#include <fstream>

class Logger {
public:
    static void error(const std::string& message);
    static std::string get_time();
    static void info(const std::string& message);
    static void initialize();
    static void warning(const std::string& message);
    static void write_to_logfile(const std::string& message);
    static void write_to_logfile_without_timestamp(const std::string& message);
};

#endif