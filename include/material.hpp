#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "utilities.hpp"
#include "hit.hpp"
#include "texture.hpp"
#include <iostream>
#include <glut.h>

// TODO (PA2): Copy from PA1.
class Material
{
public:
    explicit Material(const Vector3f &d_color = Vector3f::ZERO, const Vector3f &s_color = Vector3f::ZERO, const Vector3f &atten = Vector3f::ZERO, float s = 0, const char *texture_name = "", const char *bump_name = "") : diffuseColor(d_color), specularColor(s_color), attenuation(atten), shininess(s)
    {
        texture = Texture(texture_name);
        bumpTexture = BumpTexture(bump_name);
    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const
    {
        return diffuseColor;
    }

    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor)
    {
        Vector3f shaded = Vector3f::ZERO;
        Vector3f normal = hit.getNormal().normalized();
        Vector3f v = -ray.getDirection().normalized();
        Vector3f dirToLightN = dirToLight.normalized();
        Vector3f r = 2 * Vector3f::dot(normal, dirToLightN) * normal - dirToLightN;
        shaded = lightColor * (diffuseColor * clamp(Vector3f::dot(dirToLightN, normal)) + specularColor * pow(clamp(Vector3f::dot(v, r)), shininess));
        return shaded;
    }

    inline virtual bool Scatter(const Ray &ray, const Hit &hit, Vector3f &attenuation, Ray &scattered) = 0;

    virtual Vector3f Emission() const
    {
        return specularColor;
    }

    virtual float getRefractionRate()
    {
        return 0;
    }

    virtual Vector3f getAttenuation(float u, float v)
    {
        return attenuation;
    }

    // For OpenGL, this is fully implemented
    void Use()
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4f(diffuseColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4f(specularColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Vector2f(shininess * 4.0, 1.0f));
    }

    float getFuzz() const
    {
        return fuzz;
    }

    bool hasBump() const
    {
        return bumpTexture.hasTexture();
    }

    BumpTexture& getBump()
    {
        return bumpTexture;
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    Vector3f attenuation;
    float shininess;
    float fuzz;
    float refractionRate;
    Texture texture;
    BumpTexture bumpTexture;
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(const Vector3f &d_color = Vector3f::ZERO, const Vector3f &s_color = Vector3f::ZERO, const Vector3f &atten = Vector3f::ZERO, float s = 0, const char *texture_name = "", const char *bump_name = "") : Material(d_color, s_color, atten, s, texture_name, bump_name) {}
    inline bool Scatter(const Ray &ray, const Hit &hit, Vector3f &attenuation, Ray &scattered)
    {
        return false;
    }
    virtual Vector3f Emission() const override
    {
        return specularColor;
    }
};

class DiffuseMaterial : public Material
{
public:
    DiffuseMaterial(const Vector3f &d_color = Vector3f::ZERO, const Vector3f &s_color = Vector3f::ZERO, const Vector3f &atten = Vector3f::ZERO, float s = 0, const char *texture_name = "", const char *bump_name = "") : Material(d_color, s_color, atten, s, texture_name, bump_name) {}
    inline bool Scatter(const Ray &ray, const Hit &hit, Vector3f &attenuation, Ray &scattered)
    {
        Vector3f target = hit.getNormal() + generateRandomPoint();
        scattered = Ray(ray.pointAtParameter(hit.getT()), target);
        attenuation = getAttenuation(hit.getU(), hit.getV());
        return true;
    }

private:
    Vector3f getAttenuation(float u, float v)
    {
        if(texture.hasTexture())
            return texture.ColorAt(u, v);
        return this->attenuation;
    }
};

class MetalMaterial : public Material
{
public:
    MetalMaterial(const Vector3f &d_color = Vector3f::ZERO, const Vector3f &s_color = Vector3f::ZERO, const Vector3f &atten = Vector3f::ZERO, float s = 0, const char *texture_name = "", const char *bump_name = "", float fuzz = 0) : Material(d_color, s_color, atten, s, texture_name, bump_name) { this->fuzz = fuzz > 1 ? 1 : fuzz; }
    inline bool Scatter(const Ray &ray, const Hit &hit, Vector3f &attenuation, Ray &scattered)
    {
        Vector3f target = reflectRay(ray.getDirection().normalized(), hit.getNormal());
        scattered = Ray(ray.pointAtParameter(hit.getT()), target + fuzz * generateRandomPoint());
        attenuation = getAttenuation(hit.getU(), hit.getV());
        return (Vector3f::dot(scattered.getDirection(), hit.getNormal()) > 0);
    }

private:
    Vector3f reflectRay(const Vector3f &input, const Vector3f &normal)
    {
        return input - 2 * Vector3f::dot(input, normal) * normal;
    }
    Vector3f getAttenuation(float u, float v)
    {
        if(texture.hasTexture())
            return texture.ColorAt(u, v);
        return this->attenuation;
    }
};

class DielecMaterial : public Material
{
public:
    DielecMaterial(const Vector3f &d_color = Vector3f::ZERO, const Vector3f &s_color = Vector3f::ZERO, const Vector3f &atten = Vector3f::ZERO, float s = 0, const char *texture_name = "", const char *bump_name = "", float refractive = 0) : Material(d_color, s_color, atten, s, texture_name, bump_name) { this->refractionRate = refractive; }
    inline bool Scatter(const Ray &ray, const Hit &hit, Vector3f &attenuation, Ray &scattered)
    {
        attenuation = getAttenuation(hit.getU(), hit.getV());
        Vector3f unit = ray.getDirection().normalized();
        Vector3f target = reflectRay(unit, hit.getNormal());
        float index, reflect_prob, cos = -1 * Vector3f::dot(unit, hit.getNormal());
        Vector3f refract_ray;
        if (!hit.IsFrontFace())
        {
            index = refractionRate;
        }
        else
        {
            index = 1.0 / refractionRate;
        }

        if (refractRay(unit, hit.getNormal(), index, refract_ray))
        {
            reflect_prob = Schlick(cos, refractionRate);
        }
        else
        {
            reflect_prob = 1.1;
        }
        if (RAND() < reflect_prob)
        {
            scattered = Ray(ray.pointAtParameter(hit.getT()), target);
        }
        else
        {
            scattered = Ray(ray.pointAtParameter(hit.getT()), refract_ray);
        }
        return true;
    }

    Vector3f getAttenuation(float u, float v)
    {
        if(texture.hasTexture())
            return texture.ColorAt(u, v);
        return this->attenuation;
    }

    virtual float getRefractionRate() override
    {
        return refractionRate;
    }

private:
    Vector3f reflectRay(const Vector3f &input, const Vector3f &normal)
    {
        return input - 2 * Vector3f::dot(input, normal) * normal;
    }
    bool refractRay(const Vector3f &input, const Vector3f &normal, float index, Vector3f &refracted)
    {
        Vector3f unit = input;
        float t = -1 * Vector3f::dot(unit, normal);
        float disc = 1.0 - index * index * (1 - t * t);
        if (disc > 0)
        {
            refracted = index * (unit + t * normal) - normal * sqrt(disc);
            return true;
        }
        return false;
    }
    float Schlick(float cos, float index)
    {
        float r = ((1 - index) / (1 + index)) * ((1 - index) / (1 + index));
        return r + (1 - r) * pow((1 - cos), 5);
    }
};

#endif // MATERIAL_H
