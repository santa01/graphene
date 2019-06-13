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

#define LogFormat(format, ...)  Logger::getInstance().formatMessage("%s:%d: " format, __FILE__, __LINE__, __VA_ARGS__)
#define LogError(format, ...)   Logger::getInstance().log(Graphene::LogLevel::LOG_ERROR, "%s:%d: " format, __FILE__, __LINE__, __VA_ARGS__)
#define LogWarn(format, ...)    Logger::getInstance().log(Graphene::LogLevel::LOG_WARN,  "%s:%d: " format, __FILE__, __LINE__, __VA_ARGS__)
#define LogInfo(format, ...)    Logger::getInstance().log(Graphene::LogLevel::LOG_INFO,  "%s:%d: " format, __FILE__, __LINE__, __VA_ARGS__)
#define LogDebug(format, ...)   Logger::getInstance().log(Graphene::LogLevel::LOG_DEBUG, "%s:%d: " format, __FILE__, __LINE__, __VA_ARGS__)

namespace Graphene {

enum class LogLevel {
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
};

class Logger: public NonCopyable {
public:
    GRAPHENE_API static Logger& getInstance();
    GRAPHENE_API static std::string formatMessage(const char* format, ...);

    GRAPHENE_API void setLogLevel(LogLevel logLevel);
    GRAPHENE_API void log(LogLevel level, const char* format, ...);

private:
    Logger() = default;
    static std::string formatMessage(const char* format, va_list args);

    LogLevel logLevel = LogLevel::LOG_DEBUG;
};

}  // namespace Graphene

#endif  // LOGGER_H
