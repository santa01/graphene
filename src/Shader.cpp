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
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>
#ifdef GRAPHENE_DEBUG
#include <iostream>
#endif

namespace Graphene {

GLuint Shader::activeProgram = 0;

Shader::Shader(const std::string& name) {
    std::ifstream file(name, std::ios::binary);
    if (!file.good()) {
        throw std::runtime_error("Failed to open `" + name + "'");
    }

    file.seekg(0, std::ios::end);
    int sourceLength = file.tellg();
    file.seekg(0, std::ios::beg);

    std::unique_ptr<char[]> source(new char[sourceLength]);
    file.read(source.get(), sourceLength);
    file.close();

    this->source = std::string(source.get(), sourceLength);
    this->buildShader();
}

void Shader::buildShader() {
    std::vector<GLuint> shaders;
    std::unordered_map<std::string, GLenum> shaderTypes = {
        { "#define TYPE_VERTEX\n",   GL_VERTEX_SHADER },
        { "#define TYPE_FRAGMENT\n", GL_FRAGMENT_SHADER }
    };

    for (auto& shaderType: shaderTypes) {
        std::stringstream modifiedSource;
        modifiedSource << shaderType.first;
        modifiedSource << this->source;
        shaders.push_back(this->compile(modifiedSource.str(), shaderType.second));
    }

    this->program = this->link(shaders);
}

GLuint Shader::compile(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* sourceStrings = source.c_str();

    glShaderSource(shader, 1, &sourceStrings, nullptr);
    glCompileShader(shader);

    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::unique_ptr<GLchar[]> infoLog;
    if (infoLogLength > 1) {
        infoLog.reset(new GLchar[infoLogLength]);
        glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog.get());
#ifdef GRAPHENE_DEBUG
        std::cout << "Shader compiled\n" << infoLog.get() << std::endl;
#endif
    }

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE) {
        glDeleteShader(shader);

        std::stringstream errorMessage;
        errorMessage << "Failed to compile shader\n" << infoLog.get();
        throw std::runtime_error(errorMessage.str());
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

    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::unique_ptr<GLchar[]> infoLog;
    if (infoLogLength > 1) {
        infoLog.reset(new GLchar[infoLogLength]);
        glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.get());
#ifdef GRAPHENE_DEBUG
        std::cout << "Shader linked\n" << infoLog.get() << std::endl;
#endif
    }

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE) {
        glDeleteProgram(program);

        std::stringstream errorMessage;
        errorMessage << "Failed to link shader\n" << infoLog.get();
        throw std::runtime_error(errorMessage.str());
    }

    return program;
}

}  // namespace Graphene
