#include "tests.h"
#include <iostream>

int main() {
    try {
        bool testsOk = runAllTests();
        return testsOk ? 0 : 1;
    } catch (const std::exception& e) {
        std::cerr << "\n Ошибка " << e.what() << std::endl;
        return 1;
    }
}