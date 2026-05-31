#pragma once
#include <string>
#include <vector>

//bool isFunction(const std::string& name);	// 判断是否为函数
//int getFunctionArgCount(const std::string& name);	// 获取函数参数数量
int precedence(const std::string& op);   // 只有声明，无函数体,获取运算符优先级（包括一元负号、函数）
std::vector<std::string> infixToPostfix(const std::string& expr);	// 中缀转后缀（输出包含操作数、运算符、函数、变量）