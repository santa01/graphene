{SHADER_VERSION}
{SHADER_TYPE}

#ifdef TYPE_VERTEX

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform mat4 modelViewProjection;
uniform mat4 normalRotation;
uniform mat4 localWorld;

smooth out vec3 fragmentPosition;
smooth out vec3 fragmentNormal;
smooth out vec2 fragmentUV;

void main() {
    vec4 vertexWorldPosition = localWorld * vec4(vertexPosition, 1.0f);
    vec4 vertexWorldNormal = normalRotation * vec4(vertexNormal, 1.0f);
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
} material;     // Unused

uniform sampler2D diffuseSampler;

smooth in vec3 fragmentPosition;    // Unused
smooth in vec3 fragmentNormal;      // Unused
smooth in vec2 fragmentUV;

layout(location = 0) out vec4 outputColor;

void main() {
    outputColor = texture(diffuseSampler, fragmentUV);
}

#endif
