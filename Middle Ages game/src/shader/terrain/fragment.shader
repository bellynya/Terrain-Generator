#version 330 core

out vec4 FragColor;
in vec4 Color;
in vec2 oTex;

uniform sampler2D gTerrainTexture;

void main()
{    
    vec4 TexColor = texture(gTerrainTexture, oTex);
    FragColor = Color * TexColor;
}