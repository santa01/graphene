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
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <memory>

namespace Graphene {

bool Shader::load(const std::string& name) {
    std::fstream file(name.c_str(), std::ios::binary | std::ios::in);
    if (!file.good()) {
        return false;
    }

    file.seekg(0, std::ios::end);
    int sourceLength = file.tellg();
    file.seekg(0, std::ios::beg);

    std::unique_ptr<char[]> source(new char[sourceLength + 1]);
    source[sourceLength] = '\0';
    file.read(source.get(), sourceLength);
    file.close();

    std::vector<GLuint> shaders;
    std::unordered_map<std::string, ShaderType> shaderTypes = {
        { "#define TYPE_VERTEX\n", ShaderType::TYPE_VERTEX },
        { "#define TYPE_FRAGMENT\n", ShaderType::TYPE_FRAGMENT }
    };

    for (auto& shaderType: shaderTypes) {
        std::stringstream modifiedSource;
        modifiedSource << shaderType.first;
        modifiedSource << source.get();
        shaders.push_back(this->compileShader(modifiedSource.str(), shaderType.second));
    }

    this->program = this->linkShader(shaders);
    return (this->program != 0);
}

GLuint Shader::compileShader(const std::string& source, GLenum type) {
    GLuint shader = glCreateShader(type);
    const char* sourceStrings = source.c_str();

    glShaderSource(shader, 1, &sourceStrings, nullptr);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE) {
        glDeleteShader(shader);
        shader = 0;
    }

    return shader;
}

GLuint Shader::linkShader(const std::vector<GLuint>& shaders) {
    GLuint program = glCreateProgram();

    for (auto& shader: shaders) {
        glAttachShader(program, shader);
        glDeleteShader(shader);
    }

    glLinkProgram(program);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE) {
        glDeleteProgram(program);
        program = 0;
    }

    return program;
}

}  // namespace Graphene
