#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <stdexcept>

class RPNCalculator {
private:
    std::stack<double> stack;

public:
    // 将数值压入栈
    void push(double value) {
        stack.push(value);
    }

    // 从栈中弹出数值
    double pop() {
        if (stack.empty()) {
            throw std::runtime_error("错误：栈为空，无法执行操作。");
        }
        double value = stack.top();
        stack.pop();
        return value;
    }

    // 执行计算
    void calculate(const std::string& operation) {
        double b = pop();
        double a = pop();
        if (operation == "+") {
            push(a + b);
        } else if (operation == "-") {
            push(a - b);
        } else if (operation == "*") {
            push(a * b);
        } else if (operation == "/") {
            if (b == 0) {
                // 将操作数重新压栈以保持状态
                push(a);
                push(b);
                throw std::runtime_error("错误：检测到除零操作。");
            }
            push(a / b);
        } else {
            // 无效操作，将操作数重新压栈
            push(a);
            push(b);
            throw std::runtime_error("错误：不支持的操作 '" + operation + "'。");
        }
    }

    // 处理输入的表达式字符串
    double process(const std::string& input) {
        std::stringstream ss(input);
        std::string token;

        while (ss >> token) {
            std::stringstream token_stream(token);
            double value;
            if (token_stream >> value) {
                // 检查流中是否还有其他字符，以处理无效数字格式
                if (token_stream.eof()) {
                    push(value);
                } else {
                    throw std::runtime_error("错误：无效的数字格式 '" + token + "'。");
                }
            } else if (token == "+" || token == "-" || token == "*" || token == "/") {
                try {
                    calculate(token);
                } catch (const std::runtime_error& e) {
                    // 重新抛出异常以包含更多上下文
                    throw std::runtime_error(std::string(e.what()) + " 在处理 '" + token + "' 时发生。");
                }
            } else {
                throw std::runtime_error("错误：无法识别的输入 '" + token + "'。");
            }
        }

        if (stack.size() != 1) {
            throw std::runtime_error("错误：表达式计算结束后，栈中应仅剩一个结果。");
        }
        return stack.top();
    }

    // 清空栈
    void clear() {
        std::stack<double> empty_stack;
        stack.swap(empty_stack);
    }

    // 显示当前栈内容
    void display() const {
        std::cout << "当前栈内容: ";
        if (stack.empty()) {
            std::cout << "[空]" << std::endl;
            return;
        }
        std::stack<double> temp = stack;
        std::vector<double> elements;
        while (!temp.empty()) {
            elements.push_back(temp.top());
            temp.pop();
        }
        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    RPNCalculator calculator;
    std::string line;

    std::cout << "C++ RPN 计算器" << std::endl;
    std::cout << "输入表达式 (例, '5 5 +'), 或 'q' 退出." << std::endl;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line) || line == "q") {
            break;
        }

        if (line.empty()) {
            continue;
        }

        try {
            double result = calculator.process(line);
            std::cout << "结果: " << result << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
        // 每个表达式计算后清空栈，以支持连续独立的计算
        calculator.clear();
    }

    return 0;
}
