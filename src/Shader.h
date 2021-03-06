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

#ifndef SHADER_H
#define SHADER_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <OpenGL.h>
#include <Mat4.h>
#include <Mat3.h>
#include <Vec4.h>
#include <Vec3.h>
#include <unordered_map>
#include <string>
#include <vector>

#define TOKEN_VERSION "{SHADER_VERSION}"
#define TOKEN_TYPE    "{SHADER_TYPE}"

namespace Graphene {

class Shader: public NonCopyable {
public:
    GRAPHENE_API Shader(const std::string& shaderSource);
    GRAPHENE_API ~Shader();

    GRAPHENE_API void setUniform(const std::string& name, const Math::Mat4& value);
    GRAPHENE_API void setUniform(const std::string& name, const Math::Mat3& value);
    GRAPHENE_API void setUniform(const std::string& name, const Math::Vec4& value);
    GRAPHENE_API void setUniform(const std::string& name, const Math::Vec3& value);
    GRAPHENE_API void setUniform(const std::string& name, float value);
    GRAPHENE_API void setUniform(const std::string& name, int value);
    GRAPHENE_API void setUniformBlock(const std::string& name, int bindPoint);

    GRAPHENE_API GLuint getVersion() const;
    GRAPHENE_API const std::string& getSource() const;

    GRAPHENE_API const std::string& getName() const;
    GRAPHENE_API void setName(const std::string& shaderName);

    GRAPHENE_API void enable();

private:
    GLint checkoutUniform(const std::string& name);
    GLuint checkoutUniformBlock(const std::string& name);

    void buildShader();
    GLuint compile(const std::string& source, GLenum type);
    GLuint link(const std::vector<GLuint>& shaders);

    void queryUniforms();
    void queryUniformBlocks();

    std::unordered_map<std::string, GLint> uniforms;
    std::unordered_map<std::string, GLuint> uniformBlocks;
    std::unordered_map<std::string, GLenum> shaderTypes;

    GLuint program = 0;
    static GLuint activeProgram;

    GLuint version = 330;
    std::string shaderSource;
    std::string shaderName;
};

}  // namespace Graphene

#endif  // SHADER_H
