#pragma once

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
#include "kdtree.hpp"

#include <omp.h>
#include <sstream>

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
        // std::cout << "have intersection!" << std::endl;
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
        omp_set_num_threads(NUM_THREADS);
        for(int x=0;x<camera->getWidth();x++)
        {
            fprintf(stdout, "rendering column %d\n", x);
            // #pragma omp parallel for schedule(dynamic, 1) 
            for(int y=0;y<camera->getHeight();y++)
            {
                // printf("===================\n");
                // printf("x=%d y=%d\n", x,y);
                Vector3f color = Vector3f::ZERO;
                for(int _=0;_<sampleCnt;_++)
                {
                    float xx = float(x + (RAND() - 0.5));
                    float yy = float(y + (RAND() - 0.5));
                    Ray camRay = camera->generateRay(Vector2f(xx, yy));
                    color += colorizer->Colorize(parser, camRay);
                }
                img.SetPixel(x, y, color / sampleCnt);
                // break;
            }
            // break;
        }
        img.SaveBMP(outputFile);
    }
};

class SPPMRenderer: public Renderer
{
    std::vector<Object3D*> lightSources;
    std::vector<Hit*> visiblePoints;

    Node *root = nullptr;

    int roundCnt, photonCnt;

    void getVisiblePoints(SceneParser &parser, Ray &ray, Hit *hit)
    {
        Group *baseGroup = parser.getGroup();
        if(baseGroup == nullptr)
            return;
        int curDepth = 0;
        Vector3f totalAttenuate(1,1,1);
        while(curDepth < MAX_TRACE_DEPTH)
        {
            curDepth ++;
            hit->setT(1e38);
            if(!baseGroup->intersect(ray, *hit, eps))
            {
                hit->lightFlux += hit->attenuation * parser.getBackgroundColor();
                return;
            }

            Ray scatteredRay(ray);
            Vector3f attenuate;
            bool scatterFlag = hit->getMaterial()->Scatter(ray, *hit, attenuate, scatteredRay);
            if(scatterFlag)
                totalAttenuate = totalAttenuate * attenuate;
            if(dynamic_cast<DiffuseLight*>(hit->getMaterial()) || dynamic_cast<DiffuseMaterial*>(hit->getMaterial()))
            {
                hit->attenuation = totalAttenuate;
                hit->lightFlux += totalAttenuate * hit->getMaterial()->Emission();
                if(hit->photonCount == 0)
                {
                    hit->photonCount = 100;
                    hit->radius = 0.1;
                }
                return;
            }
            ray = scatteredRay;
        }
    }

    void photonTrace(SceneParser &parser, Ray &ray, const Vector3f &radiance)
    {
        Group *baseGroup = parser.getGroup();
        if(baseGroup == nullptr)
            return;
        int curDepth = 0;
        Vector3f totalAttenuate(radiance);
        totalAttenuate = totalAttenuate * Vector3f(1300,1300,1300);
        while(curDepth < MAX_TRACE_DEPTH)
        {
            curDepth ++;
            Hit hit;
            if(!baseGroup->intersect(ray, hit, 1e-6))
                return;
            Ray scatteredRay(ray);
            Vector3f attenuate;
            bool scatterFlag = hit.getMaterial()->Scatter(ray, hit, attenuate, scatteredRay);

            if(dynamic_cast<DiffuseMaterial*>(hit.getMaterial()))
                root->update(hit.point, totalAttenuate, hit.IsFrontFace());
            if(scatterFlag)
                totalAttenuate = totalAttenuate * attenuate;
            else
                return;
            ray = scatteredRay;
        }
    }

    void eraseTree(Node *&node)
    {
        if(node == nullptr)
            return;
        eraseTree(node->lch);
        eraseTree(node->rch);
        delete node;
        node = nullptr;
    }

    void buildTree()
    {
        if(root != nullptr)
            eraseTree(root);
        std::vector<Hit*> hitList(visiblePoints);
        root = new Node(hitList, 0, hitList.size() - 1, 0);
    }

public:

    virtual void render(SceneParser &parser, const char *outputFile)
    {
        Camera *camera = parser.getCamera();
        int width = camera->getWidth();
        int height = camera->getHeight();
        Group *baseGroup = parser.getGroup();
        lightSources = baseGroup->getLightSources();
        for(int i=0;i<width;i++)
            for(int j=0;j<height;j++)
                visiblePoints.push_back(new Hit());
        printf("Loading finished, start rendering\n");
        omp_set_num_threads(NUM_THREADS);
        for(int i=0;i<roundCnt;i++)
        {
            printf("rendering round %d\n", i);
            #pragma omp parallel for schedule(dynamic, 1) 
            for(int x=0;x<width;x++)
            {
                for(int y=0;y<height;y++)
                {
                    float xx = float(x + (RAND() - 0.5));
                    float yy = float(y + (RAND() - 0.5));
                    Ray camRay = camera->generateRay(Vector2f(xx, yy));
                    visiblePoints[x*height+y]->setT(1e38);
                    getVisiblePoints(parser, camRay, visiblePoints[x*height+y]);
                }
            }
            printf("intersection finished\n");
            fflush(stdout);
            buildTree();
            printf("finish building\n");
            int averagePhotons = photonCnt / lightSources.size();
            #pragma omp parallel for schedule(dynamic, 1) 
            for(int x=0;x<averagePhotons;x++)
            {
                for(int y=0;y<lightSources.size();y++)
                {
                    Ray r = lightSources[y]->generateRandomRay();
                    photonTrace(parser, r, lightSources[y]->getMaterial()->Emission());
                }
                if(x % 10000 == 0)
                    printf("finish rendering photon %d\n", x);
            }
            if(i <= 19 || i % 10 == 9)
            {
                Image img(width, height);
                for(int u=0;u<width;u++)
                    for(int v=0;v<height;v++)
                    {
                        Hit *hit = visiblePoints[u*height+v];
                        img.SetPixel(u, v, hit->photonFlux / (PI * hit->radius * photonCnt * (i+1)) + hit->lightFlux / (i+1));
                    }
                string tmp = outputFile;
                tmp = tmp.substr(0, tmp.size() - 4);
                std::stringstream ss;
                ss << tmp << "-" << (i + 1) << ".bmp";
                img.SaveBMP(ss.str().c_str());
            }
        }
        Image img(width, height);
        for(int u=0;u<width;u++)
            for(int v=0;v<height;v++)
            {
                Hit *hit = visiblePoints[u*height+v];
                img.SetPixel(u, v, hit->photonFlux / (PI * hit->radius * photonCnt * roundCnt) + hit->lightFlux / roundCnt);
            }
        img.SaveBMP(outputFile);
    }

    SPPMRenderer(int _roundCnt, int _photonCnt)
    {
        roundCnt = _roundCnt;
        photonCnt = _photonCnt;
        root = nullptr;
    }
};