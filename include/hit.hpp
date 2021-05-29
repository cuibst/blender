#ifndef HIT_H
#define HIT_H

#include <vecmath.h>
#include "ray.hpp"

class Material;

class Hit {
public:

    // constructors
    Hit() {
        material = nullptr;
        t = 1e38;
        photonFlux = lightFlux = Vector3f::ZERO;
        attenuation = Vector3f(1);
        radius = 5e-4;
        photonCount = 0;
    }

    Hit(float _t, Material *m, const Vector3f &n) {
        t = _t;
        material = m;
        normal = n;
        photonFlux = lightFlux = Vector3f::ZERO;
        attenuation = Vector3f(1);
        radius = 5e-4;
        photonCount = 0;
    }

    Hit(const Hit &h) {
        t = h.t;
        material = h.material;
        normal = h.normal;
        photonFlux = lightFlux = h.lightFlux;
        attenuation = h.attenuation;
        radius = h.radius;
        photonCount = 0;
    }

    // destructor
    ~Hit() = default;

    float getT() const {
        return t;
    }

    float getU() const {
        return u;
    }

    float getV() const {
        return v;
    }

    bool IsFrontFace() const {
        return isFrontFace;
    }

    Material *getMaterial() const {
        return material;
    }

    const Vector3f &getNormal() const {
        return normal;
    }

    // void set(float _t, Material *m, const Vector3f &n) {
    //     t = _t;
    //     material = m;
    //     normal = n;
    // }

    void set(float _t, float _u, float _v, Material *m, const Vector3f &n, const Ray &r) {
        t = _t;
        u = _u;
        v = _v;
        material = m;
        isFrontFace = Vector3f::dot(r.getDirection(), n) <= 0.0;
        normal = isFrontFace ? n : -n;
        point = r.pointAtParameter(_t);
    }

    void setT(float _t)
    {
        t = _t;
    }

    Vector3f lightFlux, photonFlux, attenuation;
    Vector3f point;

    float radius;
    int photonCount;

private:
    float t;
    Material *material;
    Vector3f normal;

    float u, v;
    
    bool isFrontFace;

};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

#endif // HIT_H
