#pragma once

#include <vecmath.h>
#include <string.h>
#include <stdio.h>

inline float valueClamp(float x)
{
    x -= (int)x;
    if(x < 0)
        x += 1;
    return x;
}

class Texture
{
protected:
    unsigned char *textureImage;
    int width, height;
    int channel;
public:
    Texture()
    {
        textureImage = nullptr;
        width = height = channel = 0;
    }
    Texture(const char* inputFile);
    Vector3f ColorAt(float u, float v)
    {
        if(textureImage == nullptr)
            return Vector3f::ZERO;
        u = valueClamp(u);
        v = 1 - valueClamp(v);

        int x = (int)(u * width + width) % width;
        int y = (int)(v * height + height) % height;

        auto resColor = textureImage + y * channel * width + x * channel;
        return Vector3f(resColor[0], resColor[1], resColor[2]) / 255;
    }
    bool hasTexture() const
    {
        return (textureImage != nullptr);
    }
};

class BumpTexture : public Texture
{
    float getValue(float u, float v)
    {
        if(textureImage == nullptr)
            return 0;
        int x = ((int)(u * width + width) % width + width) % width;
        int y = ((int)(v * height + height) % height + height) % height;
        return (float)(textureImage[y * channel * width + x * channel]) / 255;
    }
public:
    BumpTexture() : Texture() {}
    BumpTexture(const char *inputFile) : Texture(inputFile) {}

    Vector2f GradAt(float u, float v, float &value)
    {
        value = getValue(u, v);
        float du = 1.0f / width, dv = 1.0f / height;
        return Vector2f(10 * (getValue(u + du, v) - getValue(u - du, v)), 10 * (getValue(u, v + dv) - getValue(u, v - dv)));
    }

    bool hasBump()
    {
        return (textureImage != nullptr);
    }
};