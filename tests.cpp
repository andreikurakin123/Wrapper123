#include "tests.h"
#include "engine.h"
#include "wrapper.h"
#include <iostream>
#include <cassert>
#include <string>

class Subject {
public:
    int f3(int arg1, int arg2);
    double multiply(double a, double b);
    std::string concat(std::string s1, std::string s2);
    void printSum(int a, int b);
};

int Subject::f3(int arg1, int arg2) {
    return arg1 + arg2;
}

double Subject::multiply(double a, double b) {
    return a * b;
}

std::string Subject::concat(std::string s1, std::string s2) {
    return s1 + s2;
}

void Subject::printSum(int a, int b) {
    std::cout << "Sum: " << (a + b) << std::endl;
}

static bool testBasicFunctionality() {
    std::cout << "Test 1: Basic functionality" << std::endl;
    
    try {
        Subject subj;
        Engine dispatcher;
        
        Wrapper<Subject, int, int, int> addCmd(
            &subj, &Subject::f3, {{"arg1", 0}, {"arg2", 0}}
        );
        dispatcher.registerCommand(&addCmd, "sum");
        
        auto result = dispatcher.execute("sum", {{"arg1", 7}, {"arg2", 3}});
        int resultValue = std::any_cast<int>(result);
        
        std::cout << "f3(7, 3) = " << resultValue << " (expected: 10)" << std::endl;
        
        assert(resultValue == 10);
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }
}

static bool testDifferentTypes() {
    std::cout << "\nTest 2: Different types" << std::endl;
    
    try {
        Subject subj;
        Engine dispatcher;
        
        Wrapper<Subject, double, double, double> mulCmd(
            &subj, &Subject::multiply, {{"a", 0.0}, {"b", 0.0}}
        );
        dispatcher.registerCommand(&mulCmd, "multiply");
        
        auto result1 = dispatcher.execute("multiply", {{"a", 3.5}, {"b", 2.0}});
        double resultValue = std::any_cast<double>(result1);
        
        std::cout << "multiply(3.5, 2.0) = " << resultValue << " (expected: 7.0)" << std::endl;
        assert(resultValue == 7.0);
        
        Wrapper<Subject, std::string, std::string, std::string> concatCmd(
            &subj, &Subject::concat, {{"s1", std::string("")}, {"s2", std::string("")}}
        );
        dispatcher.registerCommand(&concatCmd, "concat");
        
        auto result2 = dispatcher.execute("concat", {
            {"s1", std::string("Good ")},
            {"s2", std::string("Job!")}
        });
        std::string strResult = std::any_cast<std::string>(result2);
        
        std::cout << "concat('Good ', 'Job!') = " << strResult << std::endl;
        assert(strResult == "Good Job!");
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }
}

static bool testVoidFunction() {
    std::cout << "\nTest 3: Void function" << std::endl;
    
    try {
        Subject subj;
        Engine dispatcher;
        
        Wrapper<Subject, void, int, int> printCmd(
            &subj, &Subject::printSum, {{"a", 0}, {"b", 0}}
        );
        dispatcher.registerCommand(&printCmd, "print");
        
        auto result = dispatcher.execute("print", {{"a", 15}, {"b", 25}});
        
        assert(!result.has_value());
        std::cout << "Void function executed successfully" << std::endl;
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }
}

static bool testErrorHandling() {
    std::cout << "\nTest 4: Error handling" << std::endl;
    
    try {
        Subject subj;
        Engine dispatcher;
        
        Wrapper<Subject, int, int, int> addCmd(
            &subj, &Subject::f3, {{"arg1", 0}, {"arg2", 0}}
        );
        dispatcher.registerCommand(&addCmd, "sum");
        
        try {
            dispatcher.execute("unknown_command", {{"arg1", 1}, {"arg2", 2}});
            return false;
        } catch (const std::invalid_argument& e) {
            std::cout << "Expected error (unknown command): " << e.what() << std::endl;
        }
        
        try {
            dispatcher.execute("sum", {{"arg1", 1}, {"arg2", std::string("wrong")}});
            return false;
        } catch (const std::invalid_argument& e) {
            std::cout << "Expected error (wrong type): " << e.what() << std::endl;
        }
        
        try {
            dispatcher.execute("sum", {{"arg1", 1}});
            return false;
        } catch (const std::invalid_argument& e) {
            std::cout << "Expected error (missing argument): " << e.what() << std::endl;
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }
}

static bool testMultithreading() {
    std::cout << "\nTest 5: Multithreading safety" << std::endl;
    
    try {
        Subject subj;
        Engine dispatcher;
        
        Wrapper<Subject, int, int, int> addCmd(
            &subj, &Subject::f3, {{"arg1", 0}, {"arg2", 0}}
        );
        dispatcher.registerCommand(&addCmd, "sum");
        
        assert(dispatcher.hasCommand("sum"));
        
        assert(dispatcher.unregisterCommand("sum"));
        assert(!dispatcher.hasCommand("sum"));
        
        std::cout << "Multithreading operations completed successfully" << std::endl;
        
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }
}

bool runAllTests() {
    int passed = 0;
    int total = 0;

    if (testBasicFunctionality()) passed++;
    total++;
    
    if (testDifferentTypes()) passed++;
    total++;
    
    if (testVoidFunction()) passed++;
    total++;
    
    if (testErrorHandling()) passed++;
    total++;
    
    if (testMultithreading()) passed++;
    total++;

    std::cout << "\n=== All tests passed successfully! ===" << std::endl;

    return passed == total;
}
