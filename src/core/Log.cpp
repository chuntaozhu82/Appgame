#include "core/Log.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Appgame {

// ConsoleLogger implementation

ConsoleLogger::ConsoleLogger(LogLevel level)
    : m_logLevel(level) {
}

void ConsoleLogger::log(LogLevel level, const std::string& message) {
    if (level < m_logLevel) {
        return;
    }

#ifdef _WIN32
    // Windows console color
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (level) {
    case LogLevel::DEBUG:
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    case LogLevel::INFO:
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case LogLevel::WARN:
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case LogLevel::ERROR:
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    case LogLevel::FATAL:
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED);
        break;
    }
    std::cout << message << std::endl;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    // ANSI color codes for Linux/Mac
    switch (level) {
    case LogLevel::DEBUG:
        std::cout << "\033[1;37m" << message << "\033[0m" << std::endl;
        break;
    case LogLevel::INFO:
        std::cout << "\033[1;32m" << message << "\033[0m" << std::endl;
        break;
    case LogLevel::WARN:
        std::cout << "\033[1;33m" << message << "\033[0m" << std::endl;
        break;
    case LogLevel::ERROR:
        std::cout << "\033[1;31m" << message << "\033[0m" << std::endl;
        break;
    case LogLevel::FATAL:
        std::cout << "\033[1;41m" << message << "\033[0m" << std::endl;
        break;
    }
#endif
}

void ConsoleLogger::setLogLevel(LogLevel level) {
    m_logLevel = level;
}

LogLevel ConsoleLogger::getLogLevel() const {
    return m_logLevel;
}

// FileLogger implementation

FileLogger::FileLogger(const std::string& filename, LogLevel level)
    : m_logLevel(level),
      m_filename(filename),
      m_file(nullptr) {
    m_file = fopen(filename.c_str(), "a");
    if (!m_file) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

FileLogger::~FileLogger() {
    if (m_file) {
        fclose(m_file);
    }
}

void FileLogger::log(LogLevel level, const std::string& message) {
    if (level < m_logLevel || !m_file) {
        return;
    }

    fprintf(m_file, "%s\n", message.c_str());
    fflush(m_file);
}

void FileLogger::setLogLevel(LogLevel level) {
    m_logLevel = level;
}

LogLevel FileLogger::getLogLevel() const {
    return m_logLevel;
}

// LogManager implementation

LogManager::LogManager()
    : m_logLevel(LogLevel::INFO),
      m_logTarget(LogTarget::CONSOLE) {
}

LogManager::~LogManager() {
    cleanup();
}

LogManager& LogManager::getInstance() {
    static LogManager instance;
    return instance;
}

bool LogManager::init(LogTarget target, const std::string& logFile) {
    cleanup();

    m_logTarget = target;
    m_logLevel = LogLevel::INFO;

    if (target == LogTarget::CONSOLE || target == LogTarget::BOTH) {
        m_consoleLogger = std::make_unique<ConsoleLogger>(m_logLevel);
    }

    if (target == LogTarget::FILE || target == LogTarget::BOTH) {
        m_fileLogger = std::make_unique<FileLogger>(logFile, m_logLevel);
    }

    return true;
}

void LogManager::cleanup() {
    m_consoleLogger.reset();
    m_fileLogger.reset();
}

void LogManager::log(LogLevel level, const std::string& message, const char* file, int line, const char* function) {
    if (level < m_logLevel) {
        return;
    }

    std::string formattedMessage = formatLogMessage(level, message, file, line, function);

    if (m_consoleLogger && (m_logTarget == LogTarget::CONSOLE || m_logTarget == LogTarget::BOTH)) {
        m_consoleLogger->log(level, formattedMessage);
    }

    if (m_fileLogger && (m_logTarget == LogTarget::FILE || m_logTarget == LogTarget::BOTH)) {
        m_fileLogger->log(level, formattedMessage);
    }
}

void LogManager::setLogLevel(LogLevel level) {
    m_logLevel = level;

    if (m_consoleLogger) {
        m_consoleLogger->setLogLevel(level);
    }

    if (m_fileLogger) {
        m_fileLogger->setLogLevel(level);
    }
}

LogLevel LogManager::getLogLevel() const {
    return m_logLevel;
}

void LogManager::setLogTarget(LogTarget target) {
    m_logTarget = target;
}

LogTarget LogManager::getLogTarget() const {
    return m_logTarget;
}

std::string LogManager::formatLogMessage(LogLevel level, const std::string& message, const char* file, int line, const char* function) {
    std::stringstream ss;
    ss << "[" << formatTimestamp() << "] "
       << "[" << levelToString(level) << "] ";

    if (file && function) {
        ss << "[" << file << ":" << line << "::" << function << "] ";
    }

    ss << message;
    return ss.str();
}

std::string LogManager::formatTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::stringstream ss;
#ifdef _WIN32
    struct tm timeinfo;
    localtime_s(&timeinfo, &now_c);
    ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count();
#else
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count();
#endif

    return ss.str();
}

std::string LogManager::levelToString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARN:
        return "WARN";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::FATAL:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
}

// Debug utility class implementation

// Performance profiling data
struct ProfilingData {
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    long long durationMs;
};

static std::unordered_map<std::string, ProfilingData> s_profilingData;
static std::mutex s_profilingMutex;

void Debug::assert(bool condition, const std::string& message, const char* file, int line) {
    if (!condition) {
        std::stringstream ss;
        ss << "Assertion failed: " << message;
        if (file) {
            ss << " in " << file;
            if (line > 0) {
                ss << ":" << line;
            }
        }
        
        LogManager::getInstance().log(LogLevel::FATAL, ss.str(), file, line, nullptr);
        
        // Break into debugger if possible
        breakpoint();
        
        // Terminate the program
        std::abort();
    }
}

void Debug::breakpoint() {
#ifdef _WIN32
    __debugbreak();
#else
    // GCC/Clang breakpoint
    __asm__ __volatile__("int $3");
#endif
}

void Debug::checkMemoryLeaks() {
#ifdef _DEBUG
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtDumpMemoryLeaks();
#else
    LogManager::getInstance().log(LogLevel::INFO, "Memory leak check not implemented for this platform");
#endif
#endif
}

void Debug::startProfiling(const std::string& name) {
    std::lock_guard<std::mutex> lock(s_profilingMutex);
    s_profilingData[name].start = std::chrono::steady_clock::now();
}

void Debug::endProfiling(const std::string& name) {
    std::lock_guard<std::mutex> lock(s_profilingMutex);
    auto it = s_profilingData.find(name);
    if (it != s_profilingData.end()) {
        it->second.end = std::chrono::steady_clock::now();
        it->second.durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            it->second.end - it->second.start).count();
    }
}

void Debug::outputProfilingResults() {
    std::lock_guard<std::mutex> lock(s_profilingMutex);
    
    LogManager::getInstance().log(LogLevel::INFO, "=== Profiling Results ===");
    for (const auto& pair : s_profilingData) {
        std::stringstream ss;
        ss << pair.first << ": " << pair.second.durationMs << " ms";
        LogManager::getInstance().log(LogLevel::INFO, ss.str());
    }
    LogManager::getInstance().log(LogLevel::INFO, "==========================");
}

} // namespace Appgame
