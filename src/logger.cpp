#include "../headers/logger.hpp"

void Logger::error(const std::string& message) {
    std::cout << "[ERROR] " << message << std::endl;
    write_to_logfile("[ERROR] " + message);
}

std::string Logger::get_time() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    char date[16] = { 0 };
    char time[16] = { 0 };

    std::strftime(date, sizeof(date), "%Y-%m-%d ", std::localtime(&now));
    std::strftime(time, sizeof(time), "%H:%M:%S", std::localtime(&now));

    return std::string(date) + std::string(time);
}

void Logger::info(const std::string& message) {
    std::cout << "[INFO] " << message << std::endl;
    write_to_logfile("[INFO] " + message);
}

void Logger::initialize() {
    char* fp = std::getenv("RAYCASTER_LOG_FILEPATH");
    const std::string FILEPATH = (fp == nullptr) ? std::string("") : std::string(fp);

    // Exit if environment variable for filepath is not set
    if (FILEPATH.empty()) {
        Logger::error("RAYCASTER_LOG_FILEPATH not set!");
        throw std::system_error(std::error_code(), "RAYCASTER_LOG_FILEPATH not set!");
    }

    write_to_logfile_without_timestamp("-----------------------------------------");
}

void Logger::warning(const std::string& message) {
    std::cout << "[WARNING] " << message << std::endl;
    write_to_logfile("[WARNING] " + message);
}

void Logger::write_to_logfile(const std::string& message) {
    std::string time_string = get_time();
    std::ofstream log_file(std::getenv("RAYCASTER_LOG_FILEPATH"), std::fstream::app);
    if (log_file.is_open())
        log_file << time_string << ' ' << message << std::endl;
}

void Logger::write_to_logfile_without_timestamp(const std::string& message) {
    std::ofstream log_file(std::getenv("RAYCASTER_LOG_FILEPATH"), std::fstream::app);
    if (log_file.is_open())
        log_file << message << std::endl;
}
