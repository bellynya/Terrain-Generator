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

#pragma once
#include "Terrain.h"
class FaultTerrainFormation : public Terrain
{
public:
	FaultTerrainFormation(std::string vertex_path, std::string fragmetn_path, int width, int height) : Terrain(vertex_path, fragmetn_path, width, height) {}

	void CreateFaultFormation(int terrain_size, int iterations, float min_height, float max_height, float filter);
private:
	struct TerrainPoint
	{
		int x;
		int z;

		void Print()
		{
			std::cout << x << ", " << z << std::endl;
		}

		bool IsEqual(TerrainPoint& p) const
		{
			return ((x == p.x) && (z == p.z));
		}
	};

	void CreateFaultFormationInternal(int iterations, float min_height, float max_height, float filter);

	void GetRandomTerrainPoint(TerrainPoint& p1, TerrainPoint& p2);

	void ApplyFIRFilter(float Filter);

	float FIRFilterSinglePoint(int x, int z, float PrevVal, float Filter);
};