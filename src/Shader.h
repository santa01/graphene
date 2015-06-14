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

#include <NonCopyable.h>
#include <Mat4.h>
#include <Mat3.h>
#include <Vec4.h>
#include <Vec3.h>
#include <GL/glew.h>
#include <unordered_map>
#include <utility>
#include <string>

#define TOKEN_VERSION "{SHADER_VERSION}"
#define TOKEN_TYPE    "{SHADER_TYPE}"

namespace Graphene {

class Shader: public NonCopyable {
public:
    Shader(const std::string& name);

    Shader(const char* source, int sourceLength) {
        this->source = std::string(source, sourceLength);
        this->version = 330;
        this->buildShader();
    }

    ~Shader() {
        if (this->program != 0) {
            glDeleteProgram(this->program);
        }
    }

    void setUniform(const std::string& name, const Math::Mat4& value) {
        GLint uniform = this->checkoutUniform(name);
        if (uniform > -1) {
            glUniformMatrix4fv(uniform, 1, GL_TRUE, (GLfloat*)value.data());
        }
    }

    void setUniform(const std::string& name, const Math::Mat3& value) {
        GLint uniform = this->checkoutUniform(name);
        if (uniform > -1) {
            glUniformMatrix3fv(uniform, 1, GL_TRUE, (GLfloat*)value.data());
        }
    }

    void setUniform(const std::string& name, const Math::Vec4& value) {
        GLint uniform = this->checkoutUniform(name);
        if (uniform > -1) {
            glUniform4fv(uniform, 1, (GLfloat*)value.data());
        }
    }

    void setUniform(const std::string& name, const Math::Vec3& value) {
        GLint uniform = this->checkoutUniform(name);
        if (uniform > -1) {
            glUniform3fv(uniform, 1, (GLfloat*)value.data());
        }
    }

    void setUniform(const std::string& name, float value) {
        GLint uniform = this->checkoutUniform(name);
        if (uniform > -1) {
            glUniform1f(uniform, value);
        }
    }

    void setUniform(const std::string& name, int value) {
        GLint uniform = this->checkoutUniform(name);
        if (uniform > -1) {
            glUniform1i(uniform, value);
        }
    }

    void setUniformBlock(const std::string& name, int bindPoint) {
        GLuint uniformBlock = this->checkoutUniformBlock(name);
        if (uniformBlock != GL_INVALID_INDEX) {
            glUniformBlockBinding(this->program, uniformBlock, bindPoint);
        }
    }

    const std::string& getSource() const {
        return this->source;
    }

    GLuint getVersion() const {
        return this->version;
    }

    void enable() {
        if (this->program != Shader::activeProgram) {
            Shader::activeProgram = this->program;
            glUseProgram(this->program);
        }
    }

private:
    GLint checkoutUniform(const std::string& name) {
        this->enable();

        if (this->uniforms.find(name) == this->uniforms.end()) {
            this->uniforms.insert(std::make_pair(name, glGetUniformLocation(this->program, name.c_str())));
        }

        return this->uniforms.at(name);
    }

    GLuint checkoutUniformBlock(const std::string& name) {
        this->enable();

        if (this->uniformBlocks.find(name) == this->uniformBlocks.end()) {
            this->uniformBlocks.insert(std::make_pair(name, glGetUniformBlockIndex(this->program, name.c_str())));
        }

        return this->uniformBlocks.at(name);
    }

    void buildShader();
    GLuint compile(const std::string& source, GLenum type);
    GLuint link(const std::vector<GLuint>& shaders);

    std::unordered_map<std::string, GLint> uniforms;
    std::unordered_map<std::string, GLuint> uniformBlocks;
    std::string source;

    static GLuint activeProgram;
    GLuint program;
    GLuint version;
};

}  // namespace Graphene

#endif  // SHADER_H
