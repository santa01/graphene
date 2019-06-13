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

#include <Logger.h>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cstring>

namespace Graphene {

std::unordered_map<LogLevel, std::string> levelNames = {
    { LogLevel::LOG_ERROR, "[E]" },
    { LogLevel::LOG_WARN,  "[W]" },
    { LogLevel::LOG_INFO,  "[I]" },
    { LogLevel::LOG_DEBUG, "[D]" }
};

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

std::string Logger::formatMessage(const char* format, ...) {
    va_list args;
    va_start(args, format);

    std::ostringstream messageStream;
    messageStream << Logger::formatMessage(format, args);

    va_end(args);

    return messageStream.str();
}

void Logger::setLogLevel(LogLevel logLevel) {
    this->logLevel = logLevel;
}

void Logger::log(LogLevel level, const char* format, ...) {
    if (level > this->logLevel) {
        return;
    }

    va_list args;
    va_start(args, format);

    std::ostringstream messageStream;
    messageStream << levelNames.at(level) << " " << Logger::formatMessage(format, args) << std::endl;

    va_end(args);

    FILE* stream = (level == LogLevel::LOG_ERROR) ? stderr : stdout;
    fprintf(stream, messageStream.str().c_str());
}

std::string Logger::formatMessage(const char* format, va_list args) {
    std::vector<char> messageData(vsnprintf(nullptr, 0, format, args) + 1);
    vsnprintf(messageData.data(), messageData.size(), format, args);

    return messageData.data();
}

}  // namespace Graphene
