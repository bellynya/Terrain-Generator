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

#ifndef STB_IMAGE_H
#define STB_IMAGE_H


#include "glm/glm.hpp"

class STBImage {
public:
    STBImage() {}

    ~STBImage() {
        if (m_imageData) {
            Unload();
        }
    }

    void Load(const char* pFilename);

    void Unload();

    glm::vec3 GetColor(int x, int y) const;

    int m_width = 0;
    int m_height = 0;
    int m_bpp = 0;
    unsigned char* m_imageData = NULL;
};


#endif