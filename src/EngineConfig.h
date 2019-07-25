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

#ifndef ENGINECONFIG_H
#define ENGINECONFIG_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <string>

#define GetEngineConfig() EngineConfig::getInstance()
#define FormatOption(width, name, value) std::setw((width)) << std::left << (name) << " : " << (value) << '\n'

namespace Graphene {

class EngineConfig: public NonCopyable {
public:
    GRAPHENE_API static EngineConfig& getInstance();
    GRAPHENE_API std::string toString() const;

    GRAPHENE_API float getFov() const;
    GRAPHENE_API void setFov(float fov);

    GRAPHENE_API int getHeight() const;
    GRAPHENE_API void setHeight(int height);

    GRAPHENE_API int getWidth() const;
    GRAPHENE_API void setWidth(int width);

    GRAPHENE_API int getSamples() const;
    GRAPHENE_API void setSamples(int samples);

    GRAPHENE_API int getAnisotropy() const;
    GRAPHENE_API void setAnisotropy(int anisotropy);

    GRAPHENE_API float getMaxFps() const;
    GRAPHENE_API void setMaxFps(float maxFps);

    GRAPHENE_API bool isVsync() const;
    GRAPHENE_API void setVsync(bool vsync);

    GRAPHENE_API bool isDebug() const;
    GRAPHENE_API void setDebug(bool debug);

private:
    EngineConfig() = default;

    float fov = 75.0f;
    int height = 600;
    int width = 800;
    int samples = 0;        // TODO
    int anisotropy = 16;
    float maxFps = 0.0f;
    bool vsync = false;
    bool debug = true;
};

}  // namespace Graphene

#endif  // ENGINECONFIG_H
