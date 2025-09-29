#include "environment.hpp"

#include <array>
#include <expected>
#include <format>
#include <string>
#include <unordered_map>

std::expected<std::unordered_map<std::string, std::string>, std::string> load_environment_variables(const std::array<std::string, 2>& variable_names) {
    std::unordered_map<std::string, std::string> env_vars;
    for(const auto& var : variable_names) {
        const char* env_var = std::getenv(var.c_str());
        if(!env_var) {
            return std::unexpected(std::format("Error: {} not set!", var));
        }

        env_vars[var] = std::string(env_var);
    }

    return env_vars;
}

