#version 330 core

uniform sampler2D uTexture;
uniform float uColorMix;

uniform vec3 uKa;
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uNs;

flat in vec3 vColor;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vViewDir;

out vec4 FragColor;

void main()
{
    // Texture colors
    vec4 texColor = texture(uTexture, vTexCoord);
    vec4 colorMode = vec4(vColor.rgb, 1.0f);
    vec4 mixedColor = mix(texColor, colorMode, uColorMix);

    // Light
    // Ambient
    vec3 lightColor = vec3(1.0); // global light - white
    float ambientIntesity = 0.2;
    vec4 ambientColor = vec4(lightColor, 1.0) * ambientIntesity * vec4(uKa, 1.0);

    // Diffuse
    vec3 lightDirection = vec3(0.0, -0.5, -1.0);
    float diffuseIntesity = 0.8;
    float diffuseFactor = dot(normalize(vNormal), -lightDirection);
    vec4 diffuseColor = vec4(0.0);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(lightColor, 1.0) * diffuseIntesity * vec4(uKd, 1.0) * diffuseFactor;
    }

    // Specular
    vec3 viewDir = normalize(vViewDir);
    vec3 halfway = normalize(-lightDirection + viewDir);
    float spec = pow(max(dot(normalize(vNormal), halfway), 0.0), uNs);
    vec4 specularColor = vec4(uKs * spec * lightColor, 1.0);

    vec4 finalColor = ambientColor + diffuseColor + specularColor;

    // Texture * (ambient color + diffuse color + specular color)
    FragColor = mixedColor * finalColor;
}
