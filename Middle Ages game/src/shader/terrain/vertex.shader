#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 Tex;
 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float gMinHeight;
uniform float gMaxHeight;

out vec4 Color;
out vec2 oTex;

void main()
{
    gl_Position = projection * view * vec4(aPos, 1.0);

    float DeltaHeight = gMaxHeight - gMinHeight;

    float HeightRatio = (aPos.y - gMinHeight) / DeltaHeight;

    float c = HeightRatio * 0.8 + 0.2;

    Color = vec4(c, c, c, 1.0);

    oTex = Tex;
}