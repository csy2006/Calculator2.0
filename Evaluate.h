#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// 无符号表版本
double evaluatePostfix(const std::vector<std::string>& postfix);

// 带符号表版本
double evaluatePostfix(const std::vector<std::string>& postfix, const std::unordered_map<std::string, double>& vars);