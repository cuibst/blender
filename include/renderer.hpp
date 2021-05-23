#pragma once

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <omp.h>

using namespace std;

class Renderer
{
public:
    virtual void render(SceneParser &parser, const char *outputFile) = 0;
};

class Colorizer
{
public:
    virtual Vector3f Colorize(SceneParser &parser, const Ray &r) = 0;
};

class RayCastColorizer : public Colorizer
{
public:
    virtual Vector3f Colorize(SceneParser &parser, const Ray &r) override
    {
        Group *group = parser.getGroup();
        Hit hit;
        if(!group->intersect(r, hit, 0))
            return parser.getBackgroundColor();
        Vector3f ret = Vector3f::ZERO;
        for(int _=0;_<parser.getNumLights();_++)
        {
            Light *light = parser.getLight(_);
            Vector3f L, lightColor;
            light->getIllumination(r.pointAtParameter(hit.getT()), L, lightColor);
            ret += hit.getMaterial()->Shade(r, hit, L, lightColor);
        }
        return ret;
    } 
};

class PathTraceColorizer : public Colorizer
{
    Vector3f getColor(SceneParser &parser,const Ray &r, int depth, Vector3f attenuate)
    {
        Group *group = parser.getGroup();
        Hit hit;
        if(!group->intersect(r, hit, 0.0001))
            return parser.getBackgroundColor();
        Vector3f ret = hit.getMaterial()->Emission();
        Ray scatteredRay(r);
        Vector3f nxtAtten;
        bool scatterFlag = hit.getMaterial()->Scatter(r, hit, nxtAtten, scatteredRay);
        attenuate = attenuate * nxtAtten;
        if(attenuate.x() < 1e-3 && attenuate.y() < 1e-3 && attenuate.z() < 1e-3)
            return ret;
        if(depth < MAX_TRACE_DEPTH && scatterFlag)
            return ret + getColor(parser, scatteredRay, depth + 1, attenuate) * nxtAtten;
        return ret;
    }

public:
    virtual Vector3f Colorize(SceneParser &parser, const Ray &r) override
    {
        return getColor(parser, r, 1, Vector3f(1,1,1));
        // Group *group = parser.getGroup();
        // Hit hit;
        // if(!group->intersect(r, hit, 0.0001))
        //     return parser.getBackgroundColor();
        // Material *material = hit.getMaterial();
        // Vector3f normal = hit.getNormal().normalized();
        // Vector3f resColor = material->Emission();
        // Ray scatteredRay(r);
        // Vector3f attenuation;
        // bool scatterFlag = true;
        // if(dynamic_cast<DiffuseMaterial*>(material) != nullptr)
        // {
        //     Vector3f newDir = (normal + generateRandomPoint()).normalized();
        //     scatteredRay = Ray(r.pointAtParameter(hit.getT()), newDir);
        //     attenuation = material->getAttenuation(hit.getU(), hit.getV());
        // }
        // else if(dynamic_cast<MetalMaterial*>(material) != nullptr)
        // {
        //     Vector3f inDir = r.getDirection().normalized();
        //     Vector3f outDir = inDir - 2 * Vector3f::dot(inDir, normal) * normal;
        //     scatteredRay = Ray(r.pointAtParameter(hit.getT()), outDir + material->getFuzz() * generateRandomPoint());
        //     attenuation = material->getAttenuation(hit.getU(), hit.getV());
        // }
        // else if(dynamic_cast<DielecMaterial*>(material) != nullptr)
        // {
        //     attenuation = material->getAttenuation(hit.getU(), hit.getV());
        //     float refRate = material->getRefractionRate();
        //     Vector3f inDir = r.getDirection().normalized();
        //     Vector3f outDir = inDir - 2 * Vector3f::dot(inDir, normal) * normal;
        //     float index, reflectProb, cos = -1 * Vector3f::dot(inDir, normal);
        //     Vector3f refractDir;
        //     if(!hit.IsFrontFace())
        //         index = refRate;
        //     else
        //         index = 1 / refRate;
        //     bool refractFlag = false;
        //     float t = -1 * Vector3f::dot(inDir, normal);
        //     float disc = 1.0 - sqr(index) * (1 - sqr(t));

        //     if(disc > 0)
        //     {
        //         refractFlag = true;
        //         refractDir = index * (inDir + t * normal) - normal * sqrt(disc);
        //     }

        //     if(refractFlag)
        //     {
        //         float r = sqr((1 - index) / (1 + index));
        //         reflectProb = r + (1 - r) * pow((1 - cos), 5);
        //     }
        //     else
        //         reflectProb = 1.1;
        //     if(drand48() < reflectProb)
        //         scatteredRay = Ray(r.pointAtParameter(hit.getT()), outDir);
        //     else
        //         scatteredRay = Ray(r.pointAtParameter(hit.getT()), refractDir);
        // }
        // else if(dynamic_cast<DiffuseLight*>(material) != nullptr)
        //     scatterFlag = false;
        // if(scatterFlag)
        // {
        //     if(attenuation.x() < 1e-3 && attenuation.y() < 1e-3 && attenuation.z() < 1e-3)
        //         return resColor;
        //     return resColor + attenuation * getColor(parser, scatteredRay, 1, attenuation);
        // }
        // return resColor;
    }
};

#define USE_PATH_TRACE 0
#define USE_RAY_CAST 1

class PathTraceRender : public Renderer
{
    int sampleCnt;
    Colorizer *colorizer;

public:
    PathTraceRender(int _sampleCnt, int mode = USE_RAY_CAST) : sampleCnt(_sampleCnt)
    {
        if(mode == USE_RAY_CAST)
            colorizer = new RayCastColorizer();
        else if(mode == USE_PATH_TRACE)
            colorizer = new PathTraceColorizer();
        else
            throw "invalid colorizer";
    }

    virtual void render(SceneParser &parser, const char *outputFile)
    {
        Camera *camera = parser.getCamera();
        Image img(camera->getWidth(), camera->getHeight());
        omp_set_num_threads(10);
        for(int x=0;x<camera->getWidth();x++)
        {
            #pragma omp parallel for schedule(dynamic, 1) 
            for(int y=0;y<camera->getHeight();y++)
            {
                Vector3f color = Vector3f::ZERO;
                for(int _=0;_<sampleCnt;_++)
                {
                    float xx = float(x + (RAND() - 0.5));
                    float yy = float(y + (RAND() - 0.5));
                    Ray camRay = camera->generateRay(Vector2f(xx, yy));
                    color += colorizer->Colorize(parser, camRay);
                }
                img.SetPixel(x, y, color / sampleCnt);
            }
            fprintf(stdout, "end rendering row %d with thread %d\n", x, omp_get_thread_num());
        }
        img.SaveBMP(outputFile);
    }
};