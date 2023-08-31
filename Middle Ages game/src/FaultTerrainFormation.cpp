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

#include "FaultTerrainFormation.h"

void FaultTerrainFormation::CreateFaultFormation(int terrain_size, int iterations, float min_height, float max_height, float filter)
{
	terrain_size_ = terrain_size;
	min_height_ = min_height;
	max_height_ = max_height;

    shader->use();
    shader->setFloat("gMinHeight", min_height);
    shader->setFloat("gMaxHeight", max_height);

	height_map.InitArray2D(terrain_size, terrain_size, 0.0f);

    CreateFaultFormationInternal(iterations, min_height, max_height, filter);

    height_map.Normalize(min_height, max_height);

    this->Init();
}

void FaultTerrainFormation::CreateFaultFormationInternal(int iterations, float min_height, float max_height, float filter)
{
    float DeltaHeight = max_height - min_height;

    for (int CurIter = 0; CurIter < iterations; CurIter++) {
        float IterationRatio = ((float)CurIter / (float)iterations);
        float Height = max_height - IterationRatio * DeltaHeight;

        TerrainPoint p1, p2;

        GetRandomTerrainPoint(p1, p2);

        int DirX = p2.x - p1.x;
        int DirZ = p2.z - p1.z;

        for (int z = 0; z < terrain_size_; z++) {
            for (int x = 0; x < terrain_size_; x++) {
                int DirX_in = x - p1.x;
                int DirZ_in = z - p1.z;

                int CrossProduct = DirX_in * DirZ - DirX * DirZ_in;

                if (CrossProduct > 0) {
                    float CurHeight = height_map.Get(x, z);
                    height_map.Set(x, z, CurHeight + Height);
                }
            }
        }
    }
    ApplyFIRFilter(filter);
}

void FaultTerrainFormation::GetRandomTerrainPoint(TerrainPoint& p1, TerrainPoint& p2)
{
    p1.x = rand() % terrain_size_;
    p1.z = rand() % terrain_size_;

    int Counter = 0;

    do {
        p2.x = rand() % terrain_size_;
        p2.z = rand() % terrain_size_;

        if (Counter++ == 1000) {
            printf("Endless loop detected in %s:%d\n", __FILE__, __LINE__);
            assert(0);
        }
    } while (p1.IsEqual(p2));
}

void FaultTerrainFormation::ApplyFIRFilter(float Filter)
{
    for (int z = 0; z < terrain_size_; z++) {
        float PrevVal = height_map.Get(0, z);
        for (int x = 1; x < terrain_size_; x++) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // right to left
    for (int z = 0; z < terrain_size_; z++) {
        float PrevVal = height_map.Get(terrain_size_ - 1, z);
        for (int x = terrain_size_ - 2; x >= 0; x--) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // bottom to top
    for (int x = 0; x < terrain_size_; x++) {
        float PrevVal = height_map.Get(x, 0);
        for (int z = 1; z < terrain_size_; z++) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }

    // top to bottom
    for (int x = 0; x < terrain_size_; x++) {
        float PrevVal = height_map.Get(x, terrain_size_ - 1);
        for (int z = terrain_size_ - 2; z >= 0; z--) {
            PrevVal = FIRFilterSinglePoint(x, z, PrevVal, Filter);
        }
    }
}

float FaultTerrainFormation::FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter)
{
    float CurVal = height_map.Get(x, z);
    float NewVal = Filter * PrevVal + (1 - Filter) * CurVal;
    height_map.Set(x, z, NewVal);
    return NewVal;
}
