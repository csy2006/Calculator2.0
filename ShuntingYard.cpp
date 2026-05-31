//中缀转后缀Shunting Yard.cpp
#include <string>
#include <cctype>
#include <unordered_set>
#include "ShuntingYard.h"
#include "Stack.h"

bool isFunction(const std::string& token) {	// 判断是否为函数
	static const std::unordered_set<std::string> functions = { "sin", "cos", "sqrt", "pow" };
	return functions.find(token) != functions.end();
}

bool isAsciiAlpha(char c) {	//判断是否为Ascii字母
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isConstant(const std::string& token) {	//判断是否为pi或Π或e
	return token == "pi" || token == "e" || token == "Π";
}

double getConstantValue(const std::string& token) {	//将输入的pi或Π或e转换为对应的数值
	if (token == "pi" || token == "Π")
		return 3.141592653589793;
	if (token == "e")
		return 2.718281828459045;
	return 0.0;	//默认返回0.0
}

int precedence(const std::string& op){	//判断并返回运算符优先级
	if (op == "sin" || op == "cos" || op == "sqrt")
		return 4;
	if (op == "~")
		return 4;	//一元负号优先级最高
	if (op == "pow")
		return 3;
	if (op == "*" || op == "/")
		return 2;
	if(op == "+" || op == "-")
		return 1;
	return 0;
}

// 中缀表达式字符串 -> 后缀对列
std::vector<std::string> infixToPostfix(const std::string& expr){	//字符串队列  函数名  中缀表达式的常量引用
	MyStack<std::string> opStack;	//存放运算符和括号的栈
	std::vector<std::string> output;	// 存放后缀表达式结果的队列
	std::string number;	// 存储当前正在读取的数字，用于累积多位数字
	
	for(size_t i = 0; i < expr.length(); ++i){	//遍历中缀表达式中的每个字符
		char c = expr[i];	//当前字符c
		std::cout << "Processing char: '" << c << "' ASCII: " << (int)c << std::endl;	//调试：打印当前处理的字符及其ASCII码
		//处理数字和小数点
		if (c >= '0' && c <= '9' || c == '.') {
			number += c;	//累加多位数字，如"123"会分三次读取'1','2','3'，然后组成"123"
		}
		//处理字母：判断是函数名还是常量名
		else if (isAsciiAlpha(c)) {
			if (!number.empty()) {
				output.push_back(number);
				number.clear();
			}
			std::string func;
			while (i < expr.length() && (isAsciiAlpha(expr[i]) || isdigit(expr[i]))) {	//函数名可以包含字母和数字
				func += expr[i++];
			}
			i--;
			if (isFunction(func)) {	//判断是函数名直接压入栈
				opStack.push(func);
			}
			else if (isConstant(func)) {	//判断是常量名直接转换为数值加入输出队列
				output.push_back(std::to_string(getConstantValue(func)));
			}
			else {	//变量名：直接加入输出队列
				output.push_back(func);
			}
		}
		//处理非数字字符
		else{
			if(!number.empty()){	//如果number中有累积的数字
				output.push_back(number);	//将数字字符串加入输出队列
				number.clear();	//清空number，准备读取下一个数字
			}
			if(c == '('){	//处理左括号
				opStack.push(std::string(1, c));	//将左括号直接压入运算符栈
			}else if(c == ')'){	//处理右括号
				while(!opStack.empty() && opStack.top() != "(") {	//栈不为空且栈顶指针不指向 (
					output.push_back(opStack.top());	//栈顶运算符转化为字符串
					opStack.pop();	//弹出运算符
				}
				if(!opStack.empty() && opStack.top() == "(")
					opStack.pop();	//弹出左括号
				//弹出检查栈顶是否为函数名
				if (!opStack.empty() && isFunction(opStack.top())) {
					output.push_back(opStack.top());
					opStack.pop();
				}
			}else if(c == '+' || c == '*' || c == '/'){	//处理运算符
				std::cout << "Found operator: " << c << std::endl;	//调试：打印当前找到的运算符
				while(!opStack.empty() && precedence(opStack.top()) >= precedence(std::string(1, c))){	//栈不为空且栈中运算符等级大于等于目前运算符 
					output.push_back(opStack.top());	//栈顶运算符转化为字符串
					opStack.pop();	//弹出栈中优先级更高或相等的运算符
				}
				opStack.push(std::string(1, c));	//将当前运算符压入栈
			}
			else if (c == '-') {	//判断一元负号
				bool isUnary = (i == 0) || (expr[i - 1] == '(');	//如果当前字符是 '-'，并且它是表达式的第一个字符，或者前一个字符是 '('，则认为它是一元负号
				if (isUnary) {	//一元符号将特殊标记~压入栈
					while (!opStack.empty() && precedence(opStack.top()) >= 3) {
						output.push_back(opStack.top());
						opStack.pop();
					}
					opStack.push("~");	//使用 '~' 代表一元负号
				}
				else {	// 处理二元减号
					while (!opStack.empty() && precedence(opStack.top()) >= precedence(std::string(1, c))) {
						output.push_back(opStack.top());
						opStack.pop();
					}
					opStack.push(std::string(1, c));
				}
			}
			else {
				std::cerr << "Warning: ignoring invalid character'" << c << "'" << std::endl;
				continue;	//忽略无效字符，继续处理下一个字符
			}
		}
	}
	if(!number.empty())	//处理末尾数字
		output.push_back(number);	//如果表达式以数字结尾，需要将最后一个数字加入队列
	while(!opStack.empty()){	//处理剩余运算符
		std::cout << "Popping operator: " << opStack.top() << std::endl;	//调试：打印正在弹出的运算符
		output.push_back(opStack.top());	//栈顶运算符转化为字符串
		opStack.pop();	//弹出运算符
	}
	return output;
}