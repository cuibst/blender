#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

class XYRectangle : public Object3D
{
public:
    XYRectangle() {}

    XYRectangle(float x0, float y0, float x1, float y1, float d, float n, Material *mat) : Object3D(mat), x0(x0), y0(y0), x1(x1), y1(y1), d(d), n(n) {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) override
    {

        if (fabs(r.getDirection().z()) < 1e-10)
            return false;

        auto t = (d - r.getOrigin().z()) / r.getDirection().z();

        if (t < tmin || t > h.getT())
            return false;

        auto x = r.getOrigin().x() + t * r.getDirection().x();
        auto y = r.getOrigin().y() + t * r.getDirection().y();
        if (x < x0 || x > x1 || y < y0 || y > y1)
            return false;

        h.set(t, (x - x0) / (x1 - x0), (y - y0) / (y1 - y0), material, getNorm((x - x0) / (x1 - x0), (y - y0) / (y1 - y0)), r);

        return true;
    }

    Vector3f getNorm(float u, float v)
    {
        return n * Vector3f(0, 0, 1);
    }

private:
    float x0, y0, x1, y1, d, n;
};

class XZRectangle : public Object3D
{
public:
    XZRectangle() {}

    XZRectangle(float x0, float z0, float x1, float z1, float d, float n, Material *mat) : Object3D(mat), x0(x0), z0(z0), x1(x1), z1(z1), d(d), n(n) {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) override
    {

        if (fabs(r.getDirection().y()) < 1e-10)
            return false;

        auto t = (d - r.getOrigin().y()) / r.getDirection().y();

        if (t < tmin || t > h.getT())
            return false;

        auto x = r.getOrigin().x() + t * r.getDirection().x();
        auto z = r.getOrigin().z() + t * r.getDirection().z();
        if (x < x0 || x > x1 || z < z0 || z > z1)
            return false;

        h.set(t, (z - z0) / (z1 - z0), (x - x0) / (x1 - x0), material, getNorm((z - z0) / (z1 - z0), (x - x0) / (x1 - x0)), r);

        return true;
    }

    Vector3f getNorm(float u, float v)
    {
        return n * Vector3f(0, 1, 0);
    }

private:
    float x0, z0, x1, z1, d, n;
};

class YZRectangle : public Object3D
{
public:
    YZRectangle() {}

    YZRectangle(float y0, float z0, float y1, float z1, float d, float n, Material *mat) : Object3D(mat), y0(y0), z0(z0), y1(y1), z1(z1), d(d), n(n) {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) override
    {

        if (fabs(r.getDirection().x()) < 1e-10)
            return false;

        auto t = (d - r.getOrigin().x()) / r.getDirection().x();

        if (t < tmin || t > h.getT())
            return false;

        auto y = r.getOrigin().y() + t * r.getDirection().y();
        auto z = r.getOrigin().z() + t * r.getDirection().z();
        if (y < y0 || y > y1 || z < z0 || z > z1)
            return false;

        h.set(t, (y - y0) / (y1 - y0), (z - z0) / (z1 - z0), material, n * Vector3f(1, 0, 0), r);

        return true;
    }

    Vector3f getNorm(float u, float v)
    {
        return n * Vector3f(1, 0, 0);
    }

private:
    float y0, z0, y1, z1, d, n;
};