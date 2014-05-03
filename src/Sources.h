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

#ifndef SOURCES_H
#define SOURCES_H

namespace Graphene {

namespace Sources {

const char geometryShader[] = R"(
    #version 330

    #ifdef TYPE_VERTEX
        uniform mat4 modelViewProjection;
        uniform mat4 localWorld;

        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec3 vertexNormal;
        layout(location = 2) in vec2 vertexUV;

        smooth out vec3 fragmentPosition;
        smooth out vec3 fragmentNormal;
        smooth out vec2 fragmentUV;

        void main () {
            vec4 vertexWorldPosition = localWorld * vec4(vertexPosition, 1.0f);
            vec4 vertexWorldNormal = localWorld * vec4(vertexNormal, 1.0f);
            gl_Position = modelViewProjection * vertexWorldPosition;

            fragmentPosition = vec3(vertexWorldPosition);
            fragmentNormal = vec3(vertexWorldNormal);
            fragmentUV = vertexUV;
        }
    #endif

    #ifdef TYPE_FRAGMENT
        layout(std140) uniform Material {
            float ambientIntensity;
            float diffuseIntensity;
            float specularIntensity;
            int specularHardness;
            vec3 diffuseColor;
            bool diffuseTexture;
            vec3 specularColor;
        } material;

        uniform sampler2D diffuseSampler;

        smooth in vec3 fragmentPosition;
        smooth in vec3 fragmentNormal;
        smooth in vec2 fragmentUV;

        layout(location = 0) out vec4 outputDiffuse;
        layout(location = 1) out vec4 outputSpecular;
        layout(location = 2) out vec4 outputPosition;
        layout(location = 3) out vec4 outputNormal;

        void main() {
            vec4 diffuseColor = vec4(material.diffuseColor, 0.0f);
            outputDiffuse = material.diffuseTexture ? texture(diffuseSampler, fragmentUV) : diffuseColor;
            outputSpecular = vec4(material.specularColor, 0.0f);
            outputPosition = vec4(fragmentPosition, 0.0f);
            outputNormal = vec4(fragmentNormal, 0.0f);
        }
    #endif
)";

const char ambientShader[] = R"(
    #version 330

    #ifdef TYPE_VERTEX
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec3 vertexNormal;
        layout(location = 2) in vec2 vertexUV;

        smooth out vec2 fragmentUV;

        void main () {
            gl_Position = vec4(vertexPosition, 1.0f);
            fragmentUV = vertexUV;
        }
    #endif

    #ifdef TYPE_FRAGMENT
        uniform vec3 ambientColor;
        uniform float ambientEnergy;

        uniform sampler2D diffuseSampler;
        uniform sampler2D specularSampler;
        uniform sampler2D positionSampler;
        uniform sampler2D normalSampler;
        uniform sampler1D depthSampler;

        smooth in vec2 fragmentUV;
        out vec4 outputColor;

        void main() {
            vec4 diffuseColor = texture(diffuseSampler, fragmentUV);
            outputColor = diffuseColor * vec4(ambientColor, 0.0f) * ambientEnergy;
        }
    #endif
)";

}  // namespace Sources

}  // namespace Graphene

#endif  // SOURCES_H
