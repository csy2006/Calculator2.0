//后缀求值Evaluate.cpp
#include "Evaluate.h"
#include "Stack.h"
#include <stdexcept>
#include <cmath>
#include <unordered_map>

//无符号版本
double evaluatePostfix(const std::vector<std::string>& postfix) {
    std::unordered_map<std::string, double>emptyMap;
    return evaluatePostfix(postfix, emptyMap);
}

// 带符号版本
double evaluatePostfix(const std::vector<std::string>& postfix, const std::unordered_map<std::string, double>& vars) {   // const 引用
    MyStack<double> valStack;
    for (const auto& token : postfix) {   // 只读遍历
        if ((token[0] >= '0' && token[0] <= '9') || (token.size() > 1 && token[0] == '-')) {
            valStack.push(std::stod(token));    // 
        }
        else if (token[0] == '~') { // 处理一元负号
			if (valStack.size() < 1)    // 一元负号需要一个操作数
                throw std::runtime_error("Not enougt operand for unary minus");
			double a = valStack.top();  //获取操作数
			valStack.pop(); //弹出操作数
			valStack.push(-a);
        }
        else if (token == "sin") {
            if (valStack.size() < 1)
                throw std::runtime_error("sin needs one operand");
            double a = valStack.top();
            valStack.pop();
            valStack.push(sin(a));
        }
        else if (token == "cos") {
            if (valStack.size() < 1)
                throw std::runtime_error("cos needs one operand");
            double a = valStack.top();
            valStack.pop();
            valStack.push(cos(a));
        }
        else if (token == "sqrt") {
            if (valStack.size() < 1)
                throw std::runtime_error("sqrt needs one operand");
            double a = valStack.top();
            valStack.pop();
            if (a < 0)
                throw std::runtime_error("sqrt of negative number");
            valStack.push(sqrt(a));
        }
        else if (token == "pow") {
            if (valStack.size() < 2)
                throw std::runtime_error("pow needs one operand");
            double a = valStack.top();
            valStack.pop();
            double b = valStack.top();
            valStack.pop();
            valStack.push(pow(a, b));
        }
        else if(token == "+" || token == "-" || token == "*" || token == "/"){  //处理二元运算符
            if (valStack.size() < 2) throw std::runtime_error("Not enough operands");
            double b = valStack.top(); valStack.pop();
            double a = valStack.top(); valStack.pop();
            switch (token[0]) {
            case '+': valStack.push(a + b); break;
            case '-': valStack.push(a - b); break;
            case '*': valStack.push(a * b); break;
            case '/':
                if (b == 0.0) 
                    throw std::runtime_error("Division by zero");
                valStack.push(a / b);
                break;
            default: throw std::runtime_error("Unknown operator: " + token);
            }
        }
        else {  // 处理变量
            auto it = vars.find(token);
            if (it == vars.end()) {
                throw std::runtime_error("Undefined variable: " + token);
            }
            valStack.push(it->second);
        }
    }
    if (valStack.size() != 1) 
        throw std::runtime_error("Invalid expression");
    return valStack.top();
}