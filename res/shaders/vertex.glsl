#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform vec3 uCameraPos;

flat out vec3 vColor;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vViewDir;

void main()
{
    vec4 worldPos = uModel * vec4(aPos, 1.0);

    vec3 color = vec3(
        fract(aPos.x) * 0.3,
        fract(aPos.y) * 0.3,
        fract(aPos.z) * 0.3
    );
    float gray = (color.r + color.g + color.b) / 1.5;
    vColor = vec3(gray);

    vTexCoord = aTexCoord;

    mat3 normalMat = transpose(inverse(mat3(uModel)));
    vNormal = normalize(normalMat * aNormal);

    vViewDir = normalize(uCameraPos - worldPos.xyz);

    gl_Position = uProjection * uView * worldPos;
}
