#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "object3d.hpp"
#include "utilities.hpp"



class Transform : public Object3D {
public:
    Transform() {}

    Transform(const Matrix4f &m, Object3D *obj) : o(obj) {
        transform = m.inverse();
    }

    ~Transform() {
    }

    virtual bool intersect(const Ray &r, Hit &h, float tmin, float T) {
        Vector3f trSource = transformPoint(transform, r.getOrigin());
        Vector3f trDirection = transformDirection(transform, r.getDirection());
        Ray tr(trSource, trDirection);
        bool inter = o->intersect(tr, h, tmin, T);
        if (inter) {
            h.set(h.getT(), h.getU(), h.getV(), h.getMaterial(), transformDirection(transform.transposed(), h.getNormal()).normalized(), r);
        }
        return inter;
    }

    void drawGL() override {
        Object3D::drawGL();
        glMatrixMode(GL_MODELVIEW); glPushMatrix();
        glMultMatrixf(transform.inverse());
        o->drawGL();
        glPopMatrix();
    }

    bool getBoundingBox(BoundingBox &box) override
    {
        BoundingBox bb;
        if(!o->getBoundingBox(bb))
            return false;
        Vector3f a = transformPoint(transform, Vector3f(box.x[0], box.y[0], box.z[0]));
        Vector3f b = transformPoint(transform, Vector3f(box.x[1], box.y[1], box.z[1]));
        box = BoundingBox(a, b);
        return true;
    }

    Material* getMaterial() const override
    {
        return o->getMaterial();
    }

protected:
    Object3D *o; //un-transformed object
    Matrix4f transform;
};

#endif //TRANSFORM_H
