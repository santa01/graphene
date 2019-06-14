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

#include <Shader.h>
#include <Logger.h>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <memory>
#include <utility>

namespace Graphene {

GLuint Shader::activeProgram = 0;

Shader::Shader(const std::string& name) {
    std::ifstream file(name, std::ios::binary);
    if (!file.good()) {
        throw std::runtime_error(LogFormat("Failed to open '%s'", name));
    }

    file.seekg(0, std::ios::end);
    std::ifstream::pos_type sourceLength = file.tellg();
    file.seekg(0, std::ios::beg);

    std::unique_ptr<char[]> source(new char[sourceLength]);
    file.read(source.get(), sourceLength);
    file.close();

    this->source = std::string(source.get(), sourceLength);
    this->version = 330;
    this->buildShader();
}

Shader::Shader(const char* source, int sourceLength) {
    this->source = std::string(source, sourceLength);
    this->version = 330;
    this->buildShader();
}

Shader::~Shader() {
    if (this->program != 0) {
        glDeleteProgram(this->program);
    }
}

void Shader::setUniform(const std::string& name, const Math::Mat4& value) {
    GLint uniform = this->checkoutUniform(name);
    if (uniform > -1) {
        glUniformMatrix4fv(uniform, 1, GL_TRUE, (GLfloat*)value.data());
    }
}

void Shader::setUniform(const std::string& name, const Math::Mat3& value) {
    GLint uniform = this->checkoutUniform(name);
    if (uniform > -1) {
        glUniformMatrix3fv(uniform, 1, GL_TRUE, (GLfloat*)value.data());
    }
}

void Shader::setUniform(const std::string& name, const Math::Vec4& value) {
    GLint uniform = this->checkoutUniform(name);
    if (uniform > -1) {
        glUniform4fv(uniform, 1, (GLfloat*)value.data());
    }
}

void Shader::setUniform(const std::string& name, const Math::Vec3& value) {
    GLint uniform = this->checkoutUniform(name);
    if (uniform > -1) {
        glUniform3fv(uniform, 1, (GLfloat*)value.data());
    }
}

void Shader::setUniform(const std::string& name, float value) {
    GLint uniform = this->checkoutUniform(name);
    if (uniform > -1) {
        glUniform1f(uniform, value);
    }
}

void Shader::setUniform(const std::string& name, int value) {
    GLint uniform = this->checkoutUniform(name);
    if (uniform > -1) {
        glUniform1i(uniform, value);
    }
}

void Shader::setUniformBlock(const std::string& name, int bindPoint) {
    GLuint uniformBlock = this->checkoutUniformBlock(name);
    if (uniformBlock != GL_INVALID_INDEX) {
        glUniformBlockBinding(this->program, uniformBlock, bindPoint);
    }
}

const std::string& Shader::getSource() const {
    return this->source;
}

GLuint Shader::getVersion() const {
    return this->version;
}

void Shader::enable() {
    if (this->program != Shader::activeProgram) {
        Shader::activeProgram = this->program;
        glUseProgram(this->program);
    }
}

GLint Shader::checkoutUniform(const std::string& name) {
    this->enable();

    if (this->uniforms.find(name) == this->uniforms.end()) {
        this->uniforms.insert(std::make_pair(name, glGetUniformLocation(this->program, name.c_str())));
    }

    return this->uniforms.at(name);
}

GLuint Shader::checkoutUniformBlock(const std::string& name) {
    this->enable();

    if (this->uniformBlocks.find(name) == this->uniformBlocks.end()) {
        this->uniformBlocks.insert(std::make_pair(name, glGetUniformBlockIndex(this->program, name.c_str())));
    }

    return this->uniformBlocks.at(name);
}

void Shader::buildShader() {
    std::vector<GLuint> shaders;
    std::unordered_map<std::string, GLenum> shaderTypes = {
        { "#define TYPE_VERTEX\n",   GL_VERTEX_SHADER },
        { "#define TYPE_FRAGMENT\n", GL_FRAGMENT_SHADER }
    };

    std::stringstream version;
    version << "#version " << this->version << "\n";

    for (auto& shaderType: shaderTypes) {
        std::string modifiedSource(this->source);
        modifiedSource.replace(modifiedSource.find(TOKEN_VERSION), sizeof(TOKEN_VERSION), version.str());
        modifiedSource.replace(modifiedSource.find(TOKEN_TYPE), sizeof(TOKEN_TYPE), shaderType.first);

        shaders.push_back(this->compile(modifiedSource, shaderType.second));
    }

    this->program = this->link(shaders);
}

GLuint Shader::compile(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* sourceStrings = source.c_str();

    glShaderSource(shader, 1, &sourceStrings, nullptr);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::unique_ptr<GLchar[]> infoLog(new GLchar[infoLogLength]);
        glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.get());
        glDeleteShader(shader);

        throw std::runtime_error(LogFormat("%s", infoLog.get()));
    }

    return shader;
}

GLuint Shader::link(const std::vector<GLuint>& shaders) {
    GLuint program = glCreateProgram();

    for (auto& shader: shaders) {
        glAttachShader(program, shader);
        glDeleteShader(shader);
    }

    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        std::unique_ptr<GLchar[]> infoLog(new GLchar[infoLogLength]);
        glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.get());
        glDeleteProgram(program);

        throw std::runtime_error(LogFormat("%s", infoLog.get()));
    }

    return program;
}

}  // namespace Graphene
