//主控函数main.cpp
#include <iostream>
#include <string>
#include <iomanip>	//设置输出精度
#include <unordered_map>
#include "Stack.h"
#include "ShuntingYard.h"
#include "Evaluate.h"

bool isAlphaAscii(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int main(){
	std::unordered_map<std::string, double>variables;
	std::string line;

	std::cout << "Calculating with variables. Enter expression or assignments. Type 'exit' to quit." << std::endl;

	while (true) {
		std::cout << "> ";
		std::getline(std::cin, line);
		if (line == "exit" || line == "quit")
			break;
		if (line.empty())
			continue;

		// 去除所有空格
		std::string expr;
		for (char c : line) {
			if (c != ' ')
				expr += c;
		}

		// ========== 调试输出 ==========
		std::cout << "Debug: expr = '" << expr << "'" << std::endl;
		for (size_t i = 0; i < expr.length(); ++i) {
			std::cout << "char " << i << ": '" << expr[i] << "' ASCII " << (int)(unsigned char)expr[i] << std::endl;
		}
		// =============================

		// 检查是否为赋值表达式
		size_t eqPos = expr.find('=');
		if (eqPos != std::string::npos) {
			std::string varName = expr.substr(0, eqPos);
			std::string rhs = expr.substr(eqPos + 1);
			// 检查变量名合法性：首字符必须是字母，且整体由字母和数字组成
			if (varName.empty() || !isAlphaAscii(varName[0])) {
				std::cerr << "Error: Invalid variable name" << std::endl;
				continue;
			}
			try {
				auto postfix = infixToPostfix(rhs);	//将赋值表达式右侧转换为后缀表达式
				double value = evaluatePostfix(postfix, variables);	//求值后缀表达式，得到变量的值
				// 调试：打印每个 token
				std::cout << "Postfix: ";	//调试：打印后缀表达式
				for (const auto& s : postfix) std::cout << s << " ";

				variables[varName] = value;	//将变量名和对应的值存入符号表
				std::cout << std::fixed << std::setprecision(10);
				std::cout << varName << " = " << value << std::endl;
			}
			catch (const std::exception& e) {
				std::cerr << "Error: " << e.what() << std::endl;	// 捕获并打印任何异常错误
			}
		}
		else {
			//普通表达式求值
			try {
				auto postfix = infixToPostfix(expr);
				double result = evaluatePostfix(postfix, variables);
				std::cout << result << std::endl;
			}
			catch(const std::exception& e){
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
	}
	return 0;
}