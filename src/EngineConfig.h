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

namespace Graphene {

class EngineConfig {
public:
    GRAPHENE_API int getFov() const;
    GRAPHENE_API void setFov(int fov);

    GRAPHENE_API int getHeight() const;
    GRAPHENE_API void setHeight(int height);

    GRAPHENE_API int getWidth() const;
    GRAPHENE_API void setWidth(int width);

    GRAPHENE_API int getSamples() const;
    GRAPHENE_API void setSamples(int samples);

    GRAPHENE_API int getMaxFps() const;
    GRAPHENE_API void setMaxFps(int maxFps);

    GRAPHENE_API bool isVsync() const;
    GRAPHENE_API void setVsync(bool vsync);

    GRAPHENE_API bool isDebug() const;
    GRAPHENE_API void setDebug(bool debug);

private:
    int fov = 75;
    int height = 600;
    int width = 800;
    int samples = 0;     // TODO
    int maxFps = 30;     // TODO
    bool vsync = false;  // TODO
    bool debug = true;
};

}  // namespace Graphene

#endif  // ENGINECONFIG_H
