// 手写实现栈
#include <iostream>
#include <string>	//字符串处理
#include <stdexcept>	// 定义标准异常类
#pragma once

template<typename T>
class MyStack {
	private:
		T* data;	//动态数组
		int capacity;	//总容量
		int topIndex;	//栈顶索引(-1为空)

		void resize() {	//栈的容量不足时，自动扩大为原来的2倍
			capacity = capacity * 2;
			T* newData = new T[capacity];
			for(int i = 0; i <= topIndex; ++i) {
				newData[i] = data[i];
			}
			delete[] data;
			data = newData;
		}
	public:
		MyStack(int initCap = 10) : capacity(initCap),topIndex(-1) {
			data = new T[capacity];
		}
		~MyStack() {
			delete[] data;
		}
		void push(const T& value) {
			if(topIndex + 1 == capacity) {
				resize();
			}
			data[++topIndex] = value;
		}
		void pop() {
			if(empty()) {
				throw std::runtime_error("Stack is empty, cannot pop");
			}
			--topIndex;
		}
		T& top() {
			if(empty()) {
				throw std::runtime_error("Stack is empty, no top");
			}
			return data[topIndex];
		}
		bool empty() const {
			return topIndex == -1;
		}
		int size() const {
			return topIndex + 1;
		}
};