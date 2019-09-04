/*
 * Copyright (c) 2013 Pavlo Lavrenenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <string>

#define GetLogger()             Logger::getInstance()
#define LogFormat(...)          GetLogger().formatLine(__FILE__, __LINE__, __VA_ARGS__)
#define LogMessage(level, ...)  GetLogger().logLine(level, __FILE__, __LINE__, __VA_ARGS__)

#define LogError(...)           LogMessage(Graphene::LogLevel::LOG_ERROR, __VA_ARGS__)
#define LogWarn(...)            LogMessage(Graphene::LogLevel::LOG_WARN, __VA_ARGS__)
#define LogInfo(...)            LogMessage(Graphene::LogLevel::LOG_INFO, __VA_ARGS__)
#define LogDebug(...)           LogMessage(Graphene::LogLevel::LOG_DEBUG, __VA_ARGS__)
#define LogTrace(...)           LogMessage(Graphene::LogLevel::LOG_TRACE, __VA_ARGS__)

namespace Graphene {

enum class LogLevel { LOG_ERROR, LOG_WARN, LOG_INFO, LOG_DEBUG, LOG_TRACE };

class Logger: public NonCopyable {
public:
    GRAPHENE_API static Logger& getInstance();
    GRAPHENE_API static std::string format(const char* format, ...);
    GRAPHENE_API static std::string formatLine(const char* file, int line, const char* format, ...);

    GRAPHENE_API void setLogLevel(LogLevel logLevel);
    GRAPHENE_API void log(LogLevel level, const char* format, ...);
    GRAPHENE_API void logLine(LogLevel level, const char* file, int line, const char* format, ...);

private:
    Logger() = default;
    static std::string formatVariadic(const char* format, va_list vlist);

    LogLevel logLevel = LogLevel::LOG_INFO;
};

}  // namespace Graphene

#endif  // LOGGER_H
