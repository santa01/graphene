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
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <memory>

namespace Graphene {

GLuint Shader::activeProgram = 0;

Shader::Shader(const std::string& shaderSource):
        shaderSource(shaderSource) {
    std::ostringstream defaultName;
    defaultName << std::hex << "Shader (0x" << this << ")";
    this->shaderName = defaultName.str();

    this->shaderTypes = {
        { "#define TYPE_VERTEX\n",   GL_VERTEX_SHADER },
        { "#define TYPE_FRAGMENT\n", GL_FRAGMENT_SHADER }
    };

    this->buildShader();
    this->queryUniforms();
    this->queryUniformBlocks();
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

GLuint Shader::getVersion() const {
    return this->version;
}

const std::string& Shader::getSource() const {
    return this->shaderSource;
}

const std::string& Shader::getName() const {
    return this->shaderName;
}

void Shader::setName(const std::string& shaderName) {
    this->shaderName = shaderName;
}

void Shader::enable() {
    if (Shader::activeProgram != this->program) {
        glUseProgram(this->program);
        Shader::activeProgram = this->program;
    }
}

GLint Shader::checkoutUniform(const std::string& name) {
    this->enable();

    auto uniformIt = this->uniforms.find(name);
    if (uniformIt == this->uniforms.end()) {
        LogWarn("Shader '%s' was unable to retrieve uniform '%s'", this->shaderName.c_str(), name.c_str());
        uniformIt = this->uniforms.emplace(name, -1).first;
    }

    return uniformIt->second;
}

GLuint Shader::checkoutUniformBlock(const std::string& name) {
    this->enable();

    auto uniformBlockIt = this->uniformBlocks.find(name);
    if (uniformBlockIt == this->uniformBlocks.end()) {
        LogWarn("Shader '%s' was unable to retrieve uniform block '%s'", this->shaderName.c_str(), name.c_str());
        uniformBlockIt = this->uniformBlocks.emplace(name, GL_INVALID_INDEX).first;
    }

    return uniformBlockIt->second;
}

void Shader::buildShader() {
    std::vector<GLuint> shaders;

    std::ostringstream version;
    version << "#version " << this->version << "\n";

    for (auto& shaderType: this->shaderTypes) {
        std::string modifiedSource(this->shaderSource);
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

void Shader::queryUniforms() {
    GLint activeUniforms = 0;
    glGetProgramiv(this->program, GL_ACTIVE_UNIFORMS, &activeUniforms);

    GLint uniformNameMaxLength = 0;
    glGetProgramiv(this->program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameMaxLength);

    uniformNameMaxLength++;
    std::unique_ptr<GLchar[]> uniformName(new GLchar[uniformNameMaxLength]);

    for (int i = 0; i < activeUniforms; i++) {
        GLsizei uniformNameLength = 0;

        glGetActiveUniformName(this->program, i, uniformNameMaxLength, &uniformNameLength, uniformName.get());
        if (uniformNameLength == 0) {
            continue;
        }

        GLint uniformLocation = glGetUniformLocation(this->program, uniformName.get());
        if (uniformLocation < 0) {
            continue;
        }

        this->uniforms.emplace(uniformName.get(), uniformLocation);
    }
}

void Shader::queryUniformBlocks() {
    GLint activeUniformBlocks = 0;
    glGetProgramiv(this->program, GL_ACTIVE_UNIFORM_BLOCKS, &activeUniformBlocks);

    GLint uniformBlockNameMaxLength = 0;
    glGetProgramiv(this->program, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &uniformBlockNameMaxLength);

    uniformBlockNameMaxLength++;
    std::unique_ptr<GLchar[]> uniformBlockName(new GLchar[uniformBlockNameMaxLength]);

    for (int i = 0; i < activeUniformBlocks; i++) {
        GLsizei uniformBlockNameLength = 0;

        glGetActiveUniformBlockName(this->program, i, uniformBlockNameMaxLength, &uniformBlockNameLength, uniformBlockName.get());
        if (uniformBlockNameLength == 0) {
            continue;
        }

        GLuint uniformBlockIndex = glGetUniformBlockIndex(this->program, uniformBlockName.get());
        if (uniformBlockIndex == GL_INVALID_INDEX) {
            continue;
        }

        this->uniformBlocks.emplace(uniformBlockName.get(), uniformBlockIndex);
    }
}

}  // namespace Graphene
