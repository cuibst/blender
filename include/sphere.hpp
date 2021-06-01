#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <glut.h>

#define sqr(x) ((x)*(x))
#define eps 1e-6

// TODO (PA2): Copy from PA1

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        center = Vector3f(0,0,0);
        radius = 1;
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material), center(center), radius(radius) {
        // 
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        //
        Vector3f originToCenter = center - r.getOrigin();
        float length1 = Vector3f::dot(r.getDirection(),originToCenter);
        float distOriginToCenter = originToCenter.length() * r.getDirection().length();
        float distCenterToRay = sqrt(sqr(distOriginToCenter) - sqr(length1));
        if(distCenterToRay > radius)
            return false;
        // std::cout<<"Ray origin:"<<r.getOrigin().x()<<" "<<r.getOrigin().y()<<" "<<r.getOrigin().z()<<std::endl;
        // std::cout<<"Ray direction:"<<r.getDirection().x()<<" "<<r.getDirection().y()<<" "<<r.getDirection().z()<<std::endl;
        // std::cout<<"Center:"<<center.x()<<" "<<center.y()<<" "<<center.z()<<std::endl;
        // std::cout<<"Radius:"<<radius<<std::endl;
        // std::cout<<distCenterToRay<<std::endl;
        float length2 = sqrt(sqr(radius) - sqr(distCenterToRay)) * r.getDirection().length();
        float actualT = (length1 - length2);
        if(originToCenter.length() < radius)
            actualT = length1 + length2;
        if(actualT < tmin + eps || actualT > h.getT())
            return false;
        Vector3f normal = (r.pointAtParameter(actualT) - center).normalized();
        float u = atan2(-normal.z(), normal.x()) / (2 * PI) + 0.5f;
        float v = acos(-normal.y()) / PI;
        h.set(actualT, u, v, material, getActualNormal(u, v, r.pointAtParameter(actualT)), r);
        return true;
    }

    void drawGL() override {
        Object3D::drawGL();
        glMatrixMode(GL_MODELVIEW); glPushMatrix();
        glTranslatef(center.x(), center.y(), center.z());
        glutSolidSphere(radius, 80, 80);
        glPopMatrix();
    }

    Ray generateRandomRay() override {
        Vector3f ret;
        do
        {
            ret = 2 * Vector3f(drand48(), drand48(), drand48()) - Vector3f(1,1,1);
        }while(ret.squaredLength() >= 1);
        ret.normalize();
        Vector3f dir = ret;
        do
        {
            ret = 2 * Vector3f(drand48(), drand48(), drand48()) - Vector3f(1,1,1);
        }while(ret.squaredLength() >= 1);
        return Ray(center + radius * dir, (ret + dir).normalized());
    }

    bool getBoundingBox(BoundingBox &box)
    {
        box = BoundingBox(center - radius * Vector3f(1,1,1), center + radius * Vector3f(1,1,1));
        return true;
    }

    Vector3f getActualNormal(float u, float v, Vector3f hitPoint)
    {
        Vector3f normal = (hitPoint - center).normalized();
        if(!material->hasBump())
            return normal;
        float value = 0;
        Vector2f grad = material->getBump().GradAt(u, v, value);
        float phi = u * 2 * PI, theta = v * 2 * PI;
        Vector3f du(-hitPoint.z(), 0, hitPoint.x()), dv(hitPoint.y() * cos(phi), -radius * sin(theta), hitPoint.y() * sin(phi));
        if(du.squaredLength() < eps)
            return normal;
        return Vector3f::cross(du + normal * grad[0] / (2 * PI), dv + normal * grad[1] / PI);
    }

protected:
    Vector3f center;
    float radius;

};


#endif
