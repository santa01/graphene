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

#include <Material.h>

namespace Graphene {

Material::Material():
        diffuseColor(1.0f, 1.0f, 1.0f),
        specularColor(1.0f, 1.0f, 1.0f) {
    this->ambientIntensity = 1.0f;
    this->diffuseIntensity = 1.0f;
    this->specularIntensity = 0.5f;
    this->specularHardness = 50;

    MaterialBuffer material = {
        this->ambientIntensity,
        this->diffuseIntensity,
        this->specularIntensity,
        this->specularHardness,
        { 0.0f, 0.0f, 0.0f }, 0,
        { 0.0f, 0.0f, 0.0f }
    };
    std::copy(this->diffuseColor.data(), this->diffuseColor.data() + 3, material.diffuseColor);
    std::copy(this->specularColor.data(), this->specularColor.data() + 3, material.specularColor);

    glGenBuffers(1, &this->ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(material), &material, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

}  // namespace Graphene
