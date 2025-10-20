#include "logger.hpp"

#include <expected>
#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

std::expected<void, std::string> init_logger() {
    try {
        auto logger = spdlog::basic_logger_mt("logger", "log/log.log", true);
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);
    }
    catch (const spdlog::spdlog_ex& ex) {
        return std::unexpected(ex.what());
    }

    spdlog::info("logger: 'logger' initialized");
    return {};
}
