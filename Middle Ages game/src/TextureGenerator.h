#ifndef TEXTURE_GENERATOR_H
#define TEXTURE_GENERATOR_H

#include <stdio.h>

#include "Texture.h"
#include "STBImage.h"

struct TextureHeightDesc {
    float Low = 0.0f;
    float Optimal = 0.0f;
    float High = 0.0f;

    void Print() const { printf("Low %f Optimal %f High %f", Low, Optimal, High); }
};


struct TextureTile {
    STBImage Image;
    TextureHeightDesc HeightDesc;
};


class Terrain;

class TextureGenerator {
public:
    TextureGenerator();

    void LoadTile(const char* Filename);

    Texture* GenerateTexture(int TextureSize, Terrain* pTerrain, float MinHeight, float MaxHeight);

private:

    void CalculateTextureRegions(float MinHeight, float MaxHeight);

    float RegionPercent(int Tile, float Height);

#define MAX_TEXTURE_TILES 4

    TextureTile m_textureTiles[MAX_TEXTURE_TILES] = {};
    int m_numTextureTiles = 0;
};

#endif