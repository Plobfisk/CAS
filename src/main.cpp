#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <ranges>
#include <algorithm>
#include <format>
#include <chrono>

#include "types.h"
#include "functions.h"
#include "lexer.h"
#include "parser.h"
#include "calculate.h"
#include "cas.h"

std::string roundString(std::string str) {
    if (str.find(".") == static_cast<size_t>(-1)) return str;
    
    auto buf = str;

    for (auto& c : std::ranges::reverse_view(str)) {
        if (c == '0') buf.pop_back();
        else break;
    }
    if (buf.back() == '.') buf.pop_back();

    return buf;
}

int main() {
    CAS cas;

    while (true) {
        try {
            std::cout << "Eval: ";
            std::string eq;
            std::getline(std::cin, eq);

            //auto start = std::chrono::system_clock::now();
            auto [var, res] = cas.calc(eq);
            //auto end = std::chrono::system_clock::now();

            //auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            //std::cout << "Calc took:" << elapsed << std::endl;

            std::cout << var << " = " << roundString(std::format("{:.5f}", res)) << '\n' << std::endl;
        }
        catch(const std::exception& e) {
            std::cerr << '\n' << e.what() << '\n';
        }
    }
    
    return 0;
}