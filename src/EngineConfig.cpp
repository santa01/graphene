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

#include <EngineConfig.h>
#include <sstream>
#include <iomanip>

namespace Graphene {

EngineConfig& EngineConfig::getInstance() {
    static EngineConfig instance;
    return instance;
}

std::string EngineConfig::toString() const {
    std::stringstream configString;

    configString << std::boolalpha << std::fixed << std::setprecision(3)
                 << FormatOption(30, "Field of view", this->fov)                   << "\n"
                 << FormatOption(30, "Window width", this->height)                 << "\n"
                 << FormatOption(30, "Window height", this->width)                 << "\n"
                 << FormatOption(30, "Antialiasing samples", this->samples)        << "\n"
                 << FormatOption(30, "Anisotropic filter level", this->anisotropy) << "\n"
                 << FormatOption(30, "FPS limit", this->maxFps)                    << "\n"
                 << FormatOption(30, "Vertical synchronization", this->vsync)      << "\n"
                 << FormatOption(30, "Debug output", this->debug);

    return configString.str();
}

float EngineConfig::getFov() const {
    return this->fov;
}

void EngineConfig::setFov(float fov) {
    this->fov = fov;
}

int EngineConfig::getHeight() const {
    return this->height;
}

void EngineConfig::setHeight(int height) {
    this->height = height;
}

int EngineConfig::getWidth() const {
    return this->width;
}

void EngineConfig::setWidth(int width) {
    this->width = width;
}

int EngineConfig::getSamples() const {
    return this->samples;
}

void EngineConfig::setSamples(int samples) {
    this->samples = samples;
}

int EngineConfig::getAnisotropy() const {
    return this->anisotropy;
}

void EngineConfig::setAnisotropy(int anisotropy) {
    this->anisotropy = anisotropy;
}

float EngineConfig::getMaxFps() const {
    return this->maxFps;
}

void EngineConfig::setMaxFps(float maxFps) {
    this->maxFps = maxFps;
}

bool EngineConfig::isVsync() const {
    return this->vsync;
}

void EngineConfig::setVsync(bool vsync) {
    this->vsync = vsync;
}

bool EngineConfig::isDebug() const {
    return this->debug;
}

void EngineConfig::setDebug(bool debug) {
    this->debug = debug;
}

}  // namespace Graphene
