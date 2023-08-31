/*
    Copyright 2022 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MidpointDispTerrain.h"
#include "glm/glm.hpp"

float RandomFloat()
{
    float Max = RAND_MAX;
    return ((float)rand() / Max);
}

float RandomFloatRange(float Start, float End)
{
    if (End == Start) {
        printf("Invalid random range: (%f, %f)\n", Start, End);
        exit(0);
    }

    float Delta = End - Start;

    float RandomValue = RandomFloat() * Delta + Start;

    return RandomValue;
}

int CalcNextPowerOfTwo(int x)
{
    int ret = 1;

    if (x == 1) {
        return 2;
    }

    while (ret < x) {
        ret = ret * 2;
    }

    return ret;
}

void MidpointDispTerrain::CreateMidpointDisplacement(int TerrainSize, float Roughness, float MinHeight, float MaxHeight)
{
    if (Roughness < 0.0f) {
        printf("%s: roughness must be positive - %f\n", __FUNCTION__, Roughness);
        exit(0);
    }

    terrain_size_ = TerrainSize;

    shader->use();
    shader->setFloat("gMinHeight", MinHeight);
    shader->setFloat("gMaxHeight", MaxHeight);

    height_map.InitArray2D(TerrainSize, TerrainSize, 0.0f);

    CreateMidpointDisplacementF32(Roughness);

    height_map.Normalize(MinHeight, MaxHeight);

    this->Init();
}


void MidpointDispTerrain::CreateMidpointDisplacementF32(float Roughness)
{
    int RectSize = CalcNextPowerOfTwo(terrain_size_);
    float CurHeight = (float)RectSize / 2.0f;
    float HeightReduce = pow(2.0f, -Roughness);

    while (RectSize > 0) {

        DiamondStep(RectSize, CurHeight);

        SquareStep(RectSize, CurHeight);

        RectSize /= 2;
        CurHeight *= HeightReduce;
    }
}


void MidpointDispTerrain::DiamondStep(int RectSize, float CurHeight)
{
    int HalfRectSize = RectSize / 2;

    for (int y = 0; y < terrain_size_; y += RectSize) {
        for (int x = 0; x < terrain_size_; x += RectSize) {
            int next_x = (x + RectSize) % terrain_size_;
            int next_y = (y + RectSize) % terrain_size_;

            if (next_x < x) {
                next_x = terrain_size_ - 1;
            }

            if (next_y < y) {
                next_y = terrain_size_ - 1;
            }

            float TopLeft = height_map.Get(x, y);
            float TopRight = height_map.Get(next_x, y);
            float BottomLeft = height_map.Get(x, next_y);
            float BottomRight = height_map.Get(next_x, next_y);

            int mid_x = (x + HalfRectSize) % terrain_size_;
            int mid_y = (y + HalfRectSize) % terrain_size_;

            float RandValue = RandomFloatRange(CurHeight, -CurHeight);
            float MidPoint = (TopLeft + TopRight + BottomLeft + BottomRight) / 4.0f;

            height_map.Set(mid_x, mid_y, MidPoint + RandValue);
        }
    }
}


void MidpointDispTerrain::SquareStep(int RectSize, float CurHeight)
{
    int HalfRectSize = RectSize / 2;

    for (int y = 0; y < terrain_size_; y += RectSize) {
        for (int x = 0; x < terrain_size_; x += RectSize) {
            int next_x = (x + RectSize) % terrain_size_;
            int next_y = (y + RectSize) % terrain_size_;

            if (next_x < x) {
                next_x = terrain_size_ - 1;
            }

            if (next_y < y) {
                next_y = terrain_size_ - 1;
            }

            int mid_x = (x + HalfRectSize) % terrain_size_;
            int mid_y = (y + HalfRectSize) % terrain_size_;

            int prev_mid_x = (x - HalfRectSize + terrain_size_) % terrain_size_;
            int prev_mid_y = (y - HalfRectSize + terrain_size_) % terrain_size_;

            float CurTopLeft = height_map.Get(x, y);
            float CurTopRight = height_map.Get(next_x, y);
            float CurCenter = height_map.Get(mid_x, mid_y);
            float PrevYCenter = height_map.Get(mid_x, prev_mid_y);
            float CurBotLeft = height_map.Get(x, next_y);
            float PrevXCenter = height_map.Get(prev_mid_x, mid_y);

            float CurLeftMid = (CurTopLeft + CurCenter + CurBotLeft + PrevXCenter) / 4.0f + RandomFloatRange(-CurHeight, CurHeight);
            float CurTopMid = (CurTopLeft + CurCenter + CurTopRight + PrevYCenter) / 4.0f + RandomFloatRange(-CurHeight, CurHeight);

            height_map.Set(mid_x, y, CurTopMid);
            height_map.Set(x, mid_y, CurLeftMid);
        }
    }
}