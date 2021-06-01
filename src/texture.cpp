#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char *inputFile)
{
    if (strcmp(inputFile, "") == 0)
    {
        textureImage = nullptr;
        return;
    }
    textureImage = stbi_load(inputFile, &width, &height, &channel, 0);
    if (!textureImage)
        fprintf(stderr, "cannot load texture file with name %s \n", inputFile);
    else
        printf("load texture file %s with size %d %d %d \n", inputFile, width, height, channel);
}