#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "ray.hpp"
#include "hit.hpp"
#include "material.hpp"
#include "bounding_box.hpp"
#include <glut.h>

// Base class for all 3d entities.
class Object3D {
public:
    Object3D() : material(nullptr) {}

    virtual ~Object3D() = default;

    explicit Object3D(Material *material) {
        this->material = material;
    }

    // PA1: Intersect Ray with this object. If hit, store information in hit structure.
    // This will not be used in PA2.
    virtual bool intersect(const Ray &r, Hit &h, float tmin, float T) = 0;

    // PA2: draw using OpenGL pipeline.
    virtual void drawGL() {
        if (material) material->Use();
    }

    virtual Material* getMaterial() const {
        return material;
    }

    virtual Ray generateRandomRay() {
        return Ray(Vector3f::ZERO, Vector3f::ZERO);
    }

    virtual bool getBoundingBox(BoundingBox &box) = 0;

protected:

    Material *material;
};

#endif

