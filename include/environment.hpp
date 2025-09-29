#ifndef RAYCASTER_ENVIRONMENT_HPP
#define RAYCASTER_ENVIRONMENT_HPP

#include <array>
#include <expected>
#include <string>
#include <unordered_map>

std::expected<std::unordered_map<std::string, std::string>, std::string> load_environment_variables(const std::array<std::string, 2>& variable_names);

#endif

