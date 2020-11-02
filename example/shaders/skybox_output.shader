{SHADER_VERSION}
{SHADER_TYPE}

#ifdef TYPE_VERTEX

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;    // Unused
layout(location = 2) in vec2 vertexUV;        // Unused

uniform mat4 modelViewProjection;
uniform mat4 normalRotation;       // Unused
uniform mat4 localWorld;           // Unused

smooth out vec3 fragmentUV;

void main() {
    // Make Z equal W. During the perspective divide Z / W produces 1.0 depth (Z) value,
    // skybox fragments appear the farthest on the scene (requires glDepthFunc(GL_LEQUAL)
    // to pass the depth test on the depth farVal​ value (1.0)).
    vec4 vertexWorldPosition = vec4(vertexPosition, 1.0f);
    gl_Position = vec4(modelViewProjection * vertexWorldPosition).xyww;

    fragmentUV = vertexPosition;
}

#endif

#ifdef TYPE_FRAGMENT

layout(std140) uniform Material {
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    int specularHardness;
    vec3 diffuseColor;
    bool hasDiffuseTexture;
    vec3 specularColor;
} material;

uniform samplerCube diffuseSampler;

smooth in vec3 fragmentUV;

layout(location = 0) out vec4 outputDiffuse;
layout(location = 1) out vec4 outputSpecular;
layout(location = 2) out vec4 outputPosition;
layout(location = 3) out vec4 outputNormal;

void main() {
    vec3 diffuseColor = material.hasDiffuseTexture ? texture(diffuseSampler, fragmentUV).rgb : material.diffuseColor;

    outputDiffuse = vec4(diffuseColor, 1.0f);
    outputSpecular = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    outputPosition = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    outputNormal = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

#endif
