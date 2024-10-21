#pragma once
#include "../vendors/nlohmann/json.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

void readJson(const std::string& path, json* j);
void readJson(const std::string& path, nlohmann::ordered_json* j);
void writeJson(const std::string& path, const json& j);
void writeJson(const std::string& path, const nlohmann::ordered_json& j);

template <typename T>
std::string pointer_to_string(T* t) {
	const void* address = static_cast<const void*>(t);
	std::stringstream ss;
	ss << address;
	std::string str_this = ss.str();
	return str_this;
}
