#include "fishing/test/TestFramework.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "Fishing Game Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    int result = RUN_ALL_TESTS();
    
    std::cout << std::endl;
    if (result == 0) {
        std::cout << "All tests passed successfully!" << std::endl;
    } else {
        std::cout << "Some tests failed. Please check the output above for details." << std::endl;
    }
    
    return result;
}
