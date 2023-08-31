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

#include "STBImage.h"

#include "stb_image.h"
#include "STBImage.h"


void STBImage::Load(const char* pFilename)
{
    if (m_imageData) {
        Unload();
    }

    stbi_set_flip_vertically_on_load(1);

    m_imageData = stbi_load(pFilename, &m_width, &m_height, &m_bpp, 0);

    if (!m_imageData) {
        printf("Can't load texture from '%s' - %s\n", pFilename, stbi_failure_reason());
        exit(0);
    }

    printf("Loaded '%s' - width %d, height %d, bpp %d\n", pFilename, m_width, m_height, m_bpp);
}


void STBImage::Unload()
{
    printf("Unloading STB image\n");
    if (!m_imageData) {
        printf("Trying to unload a NULL image\n");
        exit(0);
    }

    stbi_image_free(m_imageData);
    m_imageData = NULL;
    m_width = 0;
    m_height = 0;
    m_bpp = 0;
}


glm::vec3 STBImage::GetColor(int x, int y) const
{
    if (!m_imageData) {
        printf("%s:%d - trying to get the color but no texture was loaded\n", __FILE__, __LINE__);
        exit(0);
    }

    assert(m_width > 0);
    assert(m_height > 0);

    int WrappedX = x % m_width;
    int WrappedY = y % m_height;

    glm::vec3 Color;

    unsigned char* p = m_imageData + (WrappedY * m_width + WrappedX) * m_bpp;
    Color.r = (float)p[0];
    Color.g = (float)p[1];
    Color.b = (float)p[2];

    return Color;
}