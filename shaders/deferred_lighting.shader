{SHADER_VERSION}
{SHADER_TYPE}

#ifdef TYPE_VERTEX

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;   // Unused
layout(location = 2) in vec2 vertexUV;

smooth out vec2 fragmentUV;

void main() {
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
    int type;
    float energy;
    float falloff;
    float angle;
    float blend;
} light;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightDirection;

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

    vec3 position = positionSample.xyz;
    vec3 normal = normalize(normalSample.xyz);

    vec3 direction = (light.type == TYPE_POINT) ? position - lightPosition : lightDirection;
    direction = normalize(direction);

    float luminance = dot(-direction, normal);
    float diffuseIntensity = specularSample.a;
    vec3 diffuseColor = diffuseSample.rgb * light.color * (luminance > 0.0f ? luminance : 0.0f) * diffuseIntensity;

    vec3 cameraDirection = normalize(position - cameraPosition);
    vec3 reflectedDirection = reflect(direction, normal);

    float specularHardness = normalSample.a;
    float specularIntensity = positionSample.a;
    float highlight = pow(dot(-cameraDirection, reflectedDirection), specularHardness);
    vec3 specularColor = specularSample.rgb * (highlight > 0.0f ? highlight : 0.0f) * specularIntensity;

    float lightAttenuation = 1.0f;
    if (light.type != TYPE_DIRECTED) {
        float falloff = pow(light.falloff, 2);
        float distance = pow(distance(lightPosition, position), 2);
        lightAttenuation = falloff / (falloff + distance);
    }

    float borderAttenuation = 1.0f;
    if (light.type == TYPE_SPOT) {
        float softBorder = cos(radians(light.angle) / 2.0);
        float hardBorder = cos(radians(light.angle * (1.0 - light.blend)) / 2.0);
        float lightAngle = dot(direction, normalize(position - lightPosition));
        borderAttenuation = clamp((lightAngle - softBorder) / (hardBorder - softBorder), 0.0f, 1.0f);
    }

    outputColor = vec4(diffuseColor + specularColor, 0.0f) * light.energy * lightAttenuation * borderAttenuation;
}

#endif
