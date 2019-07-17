{SHADER_VERSION}
{SHADER_TYPE}

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
uniform sampler2D normalSampler;

smooth in vec2 fragmentUV;

layout(location = 0) out vec4 outputColor;

void main() {
    vec4 diffuseSample = texture(diffuseSampler, fragmentUV);

    vec4 diffuseColor = vec4(diffuseSample.rgb, 0.0f);
    float ambientIntensity = diffuseSample.a;

    outputColor = diffuseColor * vec4(ambientColor, 0.0f) * ambientEnergy * ambientIntensity;
}

#endif
