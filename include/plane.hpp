#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include "utilities.hpp"
#include <vecmath.h>
#include <cmath>

#define eps 1e-6

// TODO (PA2): Copy from PA1

class Plane : public Object3D {
public:
    Plane() {

    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->d = d;
        this->d /= normal.length();
        this->normal = normal.normalized();
        if(Vector3f::cross(this->normal.normalized(), Vector3f::UP).length() < eps)
            this->vDir = Vector3f::FORWARD;
        else
            this->vDir = Vector3f::UP;
        this->uDir = Vector3f::cross(this->normal.normalized(), this->vDir).normalized();
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float proj = Vector3f::dot(normal, r.getDirection());
        if(fabs(proj) < eps)
        {
            //FIXME: maybe need to check origin on the plane
            return false;
        }
        float actualT = -(Vector3f::dot(normal, r.getOrigin()) - d) / proj;
        if(actualT < tmin - eps || actualT > h.getT())
            return false;
        Vector3f hitPoint = r.pointAtParameter(actualT);
        float u = Vector3f::dot(hitPoint, uDir);
        float v = Vector3f::dot(hitPoint, vDir);
        h.set(actualT, u, v, material, normal, r);
        return true;
    }

    void drawGL() override {
        Object3D::drawGL();
        Vector3f xAxis = Vector3f::RIGHT;
        Vector3f yAxis = Vector3f::cross(normal, xAxis);
        xAxis = Vector3f::cross(yAxis, normal);
        const float planeSize = 10.0;
        glBegin(GL_TRIANGLES);
        glNormal3fv(normal);
        glVertex3fv(d * normal + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * normal - planeSize * xAxis - planeSize * yAxis);
        glVertex3fv(d * normal + planeSize * xAxis - planeSize * yAxis);
        glNormal3fv(normal);
        glVertex3fv(d * normal + planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * normal - planeSize * xAxis + planeSize * yAxis);
        glVertex3fv(d * normal - planeSize * xAxis - planeSize * yAxis);
        glEnd();
    }

protected:
    Vector3f normal;
    float d;
    Vector3f uDir, vDir;

};

#endif //PLANE_H
		
