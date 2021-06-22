#pragma once

#include <vecmath.h>
#include "object3d.hpp"
#include "utilities.hpp"

extern float timeLimit;

class Move : public Object3D {
public:
    Move() {}

    Move(const Vector3f &v, const Vector3f &a, Object3D *obj) : o(obj) {
        velocity = v;
        alpha = a;
    }

    ~Move() {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin, float T) override{
        Matrix4f transform = Matrix4f::translation(-velocity * T - 0.5f * alpha * T * T);
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
        float T = drand48();
        Matrix4f transform = Matrix4f::translation(-velocity * T);
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
        float T = 0;
        Matrix4f transform = Matrix4f::translation(-velocity * T - 0.5f * alpha * T * T);
        Vector3f a = transformPoint(transform, Vector3f(box.x[0], box.y[0], box.z[0]));
        Vector3f b = transformPoint(transform, Vector3f(box.x[1], box.y[1], box.z[1]));
        box = BoundingBox(a, b);
        T = timeLimit;
        transform = Matrix4f::translation(-velocity * T - 0.5f * alpha * T * T);
        a = transformPoint(transform, Vector3f(box.x[0], box.y[0], box.z[0]));
        b = transformPoint(transform, Vector3f(box.x[1], box.y[1], box.z[1]));
        box = BoundingBox::mergeBox(box, BoundingBox(a, b));
        if(fabs(alpha.x()) > eps)
        {
            T = -velocity.x() / alpha.x();
            if(T > 0 && T < timeLimit)
            {
                transform = Matrix4f::translation(-velocity * T - 0.5f * alpha * T * T);
                a = transformPoint(transform, Vector3f(box.x[0], box.y[0], box.z[0]));
                b = transformPoint(transform, Vector3f(box.x[1], box.y[1], box.z[1]));
                box = BoundingBox::mergeBox(box, BoundingBox(a, b));
            }
        }
        if(fabs(alpha.y()) > eps)
        {
            T = -velocity.y() / alpha.y();
            if(T > 0 && T < timeLimit)
            {
                transform = Matrix4f::translation(-velocity * T - 0.5f * alpha * T * T);
                a = transformPoint(transform, Vector3f(box.x[0], box.y[0], box.z[0]));
                b = transformPoint(transform, Vector3f(box.x[1], box.y[1], box.z[1]));
                box = BoundingBox::mergeBox(box, BoundingBox(a, b));
            }
        }
        if(fabs(alpha.z()) > eps)
        {
            T = -velocity.z() / alpha.z();
            if(T > 0 && T < timeLimit)
            {
                transform = Matrix4f::translation(-velocity * T - 0.5f * alpha * T * T);
                a = transformPoint(transform, Vector3f(box.x[0], box.y[0], box.z[0]));
                b = transformPoint(transform, Vector3f(box.x[1], box.y[1], box.z[1]));
                box = BoundingBox::mergeBox(box, BoundingBox(a, b));
            }
        }
        return true;
    }

    Material* getMaterial() const override
    {
        return o->getMaterial();
    }

protected:
    Object3D *o; //un-transformed object
    Vector3f velocity;
    Vector3f alpha;
};

