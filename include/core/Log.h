#ifndef LOG_H
#define LOG_H

#include <string>
#include <memory>
#include <functional>
#include <chrono>
#include <sstream>

namespace Appgame {

// 日志级别
enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

// 日志输出目标
enum class LogTarget {
    CONSOLE,
    FILE,
    BOTH
};

// 日志接口抽象类
class LogInterface {
public:
    virtual ~LogInterface() = default;

    // 输出日志
    virtual void log(LogLevel level, const std::string& message) = 0;

    // 设置日志级别
    virtual void setLogLevel(LogLevel level) = 0;

    // 获取当前日志级别
    virtual LogLevel getLogLevel() const = 0;
};

// 控制台日志输出类
class ConsoleLogger : public LogInterface {
public:
    ConsoleLogger(LogLevel level = LogLevel::INFO);

    void log(LogLevel level, const std::string& message) override;
    void setLogLevel(LogLevel level) override;
    LogLevel getLogLevel() const override;

private:
    LogLevel m_logLevel;
};

// 文件日志输出类
class FileLogger : public LogInterface {
public:
    FileLogger(const std::string& filename, LogLevel level = LogLevel::INFO);
    ~FileLogger();

    void log(LogLevel level, const std::string& message) override;
    void setLogLevel(LogLevel level) override;
    LogLevel getLogLevel() const override;

private:
    LogLevel m_logLevel;
    std::string m_filename;
    FILE* m_file;
};

// 日志管理器类
class LogManager {
public:
    static LogManager& getInstance();

    // 初始化日志系统
    bool init(LogTarget target = LogTarget::CONSOLE, const std::string& logFile = "appgame.log");

    // 清理日志系统
    void cleanup();

    // 输出日志
    void log(LogLevel level, const std::string& message, const char* file = nullptr, int line = 0, const char* function = nullptr);

    // 设置日志级别
    void setLogLevel(LogLevel level);

    // 获取当前日志级别
    LogLevel getLogLevel() const;

    // 设置日志输出目标
    void setLogTarget(LogTarget target);

    // 获取日志输出目标
    LogTarget getLogTarget() const;

private:
    LogManager();
    ~LogManager();

    // 格式化日志消息
    std::string formatLogMessage(LogLevel level, const std::string& message, const char* file, int line, const char* function);

    // 格式化时间戳
    std::string formatTimestamp();

    // 转换日志级别为字符串
    std::string levelToString(LogLevel level);

    // 日志接口
    std::unique_ptr<ConsoleLogger> m_consoleLogger;
    std::unique_ptr<FileLogger> m_fileLogger;

    // 配置
    LogLevel m_logLevel;
    LogTarget m_logTarget;
};

// 调试工具类
class Debug {
public:
    // 断言
    static void assert(bool condition, const std::string& message, const char* file = nullptr, int line = 0);

    // 断点
    static void breakpoint();

    // 检查内存泄漏
    static void checkMemoryLeaks();

    // 性能分析开始
    static void startProfiling(const std::string& name);

    // 性能分析结束
    static void endProfiling(const std::string& name);

    // 输出性能分析结果
    static void outputProfilingResults();
};

// 日志宏
#define LOG_DEBUG(message) LogManager::getInstance().log(LogLevel::DEBUG, message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_INFO(message) LogManager::getInstance().log(LogLevel::INFO, message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_WARN(message) LogManager::getInstance().log(LogLevel::WARN, message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_ERROR(message) LogManager::getInstance().log(LogLevel::ERROR, message, __FILE__, __LINE__, __FUNCTION__)
#define LOG_FATAL(message) LogManager::getInstance().log(LogLevel::FATAL, message, __FILE__, __LINE__, __FUNCTION__)

// 断言宏
#define ASSERT(condition, message) Debug::assert(condition, message, __FILE__, __LINE__)

// 断点宏
#define BREAKPOINT() Debug::breakpoint()

} // namespace Appgame

#endif // LOG_H
