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
        if (material.diffuseTexture) {
            outputDiffuse = vec4(texture(diffuseSampler, fragmentUV).rgb, material.diffuseIntensity);
        } else {
            outputDiffuse = vec4(material.diffuseColor, material.diffuseIntensity);
        }

        outputSpecular = vec4(material.specularColor, material.specularIntensity);
        outputPosition = vec4(fragmentPosition, material.specularHardness);
        outputNormal = vec4(fragmentNormal, 0.0f);
    }

#endif
)";

const char ambientShader[] = R"(
    #version 330

#ifdef TYPE_VERTEX

    layout(location = 0) in vec3 vertexPosition;
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

    smooth in vec2 fragmentUV;

    layout(location = 0) out vec4 outputColor;

    void main() {
        vec4 diffuseColor = vec4(texture(diffuseSampler, fragmentUV).rgb, 0.0f);
        outputColor = diffuseColor * vec4(ambientColor, 0.0f) * ambientEnergy;
    }

#endif
)";

const char lightingShader[] = R"(
    #version 330

#ifdef TYPE_VERTEX

    layout(location = 0) in vec3 vertexPosition;
    layout(location = 2) in vec2 vertexUV;

    smooth out vec2 fragmentUV;

    void main () {
        gl_Position = vec4(vertexPosition, 1.0f);
        fragmentUV = vertexUV;
    }

#endif

#ifdef TYPE_FRAGMENT

    #define TYPE_POINT    0
    #define TYPE_SPOT     1
    #define TYPE_DIRECTED 2

    layout(std140) uniform Light {
        vec3 color;
        float energy;
        vec3 position;
        float falloff;
        vec3 direction;
        float innerAngle;
        float outerAngle;
        int lightType;
    } light;

    uniform sampler2D diffuseSampler;
    uniform sampler2D specularSampler;
    uniform sampler2D positionSampler;
    uniform sampler2D normalSampler;
    uniform sampler2D depthSampler;

    smooth in vec2 fragmentUV;

    layout(location = 0) out vec4 outputColor;

    void main() {
        vec4 diffuseSample = texture(diffuseSampler, fragmentUV);
        vec4 specularSample = texture(specularSampler, fragmentUV);
        vec4 positionSample = texture(positionSampler, fragmentUV);
        vec4 normalSample = texture(normalSampler, fragmentUV);

        vec3 normal = normalize(normalSample.xyz);
        vec3 position = positionSample.xyz;

        vec3 lightDirection = (light.lightType == TYPE_POINT) ? position - light.position : light.direction;
        lightDirection = normalize(lightDirection);

        float luminance = dot(normal, -lightDirection);
        vec3 diffuseColor = light.color * (luminance > 0.0f ? luminance : 0.0f);

        float diffuseIntensity = diffuseSample.w;
        outputColor = texture(diffuseSampler, fragmentUV) + vec4(diffuseColor * diffuseIntensity, 0.0f);
    }

#endif
)";

}  // namespace Sources

}  // namespace Graphene

#endif  // SOURCES_H
