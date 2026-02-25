#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <sstream>
#include <cmath>

namespace FishingGame {
namespace Test {

struct TestResult {
    std::string testName;
    bool passed;
    std::string message;
    std::string file;
    int line;
};

class TestRunner {
public:
    static TestRunner& getInstance() {
        static TestRunner instance;
        return instance;
    }
    
    void addTest(const std::string& suiteName, const std::string& testName, std::function<void()> testFunc) {
        m_testSuites[suiteName].push_back({testName, testFunc});
    }
    
    int runAllTests() {
        int totalTests = 0;
        int passedTests = 0;
        int failedTests = 0;
        
        std::cout << "========================================" << std::endl;
        std::cout << "Running All Tests" << std::endl;
        std::cout << "========================================" << std::endl;
        
        for (auto& suite : m_testSuites) {
            std::cout << "\n--- Test Suite: " << suite.first << " ---" << std::endl;
            
            for (auto& test : suite.second) {
                totalTests++;
                std::cout << "Running test: " << test.name << "... ";
                
                try {
                    m_currentTestName = test.name;
                    m_currentTestPassed = true;
                    m_currentTestMessage.clear();
                    
                    test.func();
                    
                    if (m_currentTestPassed) {
                        passedTests++;
                        std::cout << "PASSED" << std::endl;
                    } else {
                        failedTests++;
                        std::cout << "FAILED: " << m_currentTestMessage << std::endl;
                    }
                } catch (const std::exception& e) {
                    failedTests++;
                    std::cout << "FAILED (Exception): " << e.what() << std::endl;
                } catch (...) {
                    failedTests++;
                    std::cout << "FAILED (Unknown exception)" << std::endl;
                }
            }
        }
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "Test Results Summary" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Total Tests: " << totalTests << std::endl;
        std::cout << "Passed: " << passedTests << std::endl;
        std::cout << "Failed: " << failedTests << std::endl;
        std::cout << "Skipped: 0" << std::endl;
        std::cout << "========================================" << std::endl;
        
        return failedTests > 0 ? 1 : 0;
    }
    
    void failTest(const std::string& message, const std::string& file, int line) {
        m_currentTestPassed = false;
        std::stringstream ss;
        ss << message << " (at " << file << ":" << line << ")";
        m_currentTestMessage = ss.str();
    }
    
private:
    TestRunner() : m_currentTestPassed(true) {}
    
    struct TestInfo {
        std::string name;
        std::function<void()> func;
    };
    
    std::map<std::string, std::vector<TestInfo>> m_testSuites;
    std::string m_currentTestName;
    bool m_currentTestPassed;
    std::string m_currentTestMessage;
};

class TestRegistrar {
public:
    TestRegistrar(const std::string& suiteName, const std::string& testName, std::function<void()> testFunc) {
        TestRunner::getInstance().addTest(suiteName, testName, testFunc);
    }
};

#define TEST_SUITE(suiteName) namespace TestSuite_##suiteName

#define TEST(suiteName, testName) \
    void test_##suiteName##_##testName(); \
    static FishingGame::Test::TestRegistrar registrar_##suiteName##_##testName(#suiteName, #testName, test_##suiteName##_##testName); \
    void test_##suiteName##_##testName()

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            FishingGame::Test::TestRunner::getInstance().failTest("ASSERT_TRUE failed: " #condition, __FILE__, __LINE__); \
            return; \
        } \
    } while (0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            FishingGame::Test::TestRunner::getInstance().failTest("ASSERT_FALSE failed: " #condition, __FILE__, __LINE__); \
            return; \
        } \
    } while (0)

#define ASSERT_EQ(expected, actual) \
    do { \
        if (!((expected) == (actual))) { \
            std::stringstream ss; \
            ss << "ASSERT_EQ failed: expected " << (expected) << " but got " << (actual); \
            FishingGame::Test::TestRunner::getInstance().failTest(ss.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while (0)

#define ASSERT_NE(expected, actual) \
    do { \
        if ((expected) == (actual)) { \
            std::stringstream ss; \
            ss << "ASSERT_NE failed: values are equal: " << (expected); \
            FishingGame::Test::TestRunner::getInstance().failTest(ss.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while (0)

#define ASSERT_NEAR(expected, actual, tolerance) \
    do { \
        double diff = std::abs((expected) - (actual)); \
        if (diff > (tolerance)) { \
            std::stringstream ss; \
            ss << "ASSERT_NEAR failed: expected " << (expected) << " but got " << (actual) << " (diff: " << diff << ")"; \
            FishingGame::Test::TestRunner::getInstance().failTest(ss.str(), __FILE__, __LINE__); \
            return; \
        } \
    } while (0)

#define ASSERT_NULL(ptr) \
    do { \
        if ((ptr) != nullptr) { \
            FishingGame::Test::TestRunner::getInstance().failTest("ASSERT_NULL failed: pointer is not null", __FILE__, __LINE__); \
            return; \
        } \
    } while (0)

#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == nullptr) { \
            FishingGame::Test::TestRunner::getInstance().failTest("ASSERT_NOT_NULL failed: pointer is null", __FILE__, __LINE__); \
            return; \
        } \
    } while (0)

#define RUN_ALL_TESTS() FishingGame::Test::TestRunner::getInstance().runAllTests()

}
}

#endif
