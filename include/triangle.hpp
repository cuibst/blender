#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>

#define eps 1e-6

using namespace std;

// TODO (PA2): Copy from PA1
class Triangle: public Object3D
{
	BoundingBox box;
	Vector3f uDir, vDir;

public:
	Triangle() = delete;
        ///@param a b c are three vertex positions of the triangle

	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		vertices[0] = a;
		vertices[1] = b;
		vertices[2] = c;
		normal = Vector3f::cross(b-a, c-a).normalized();
		Vector3f min_point(fmin(vertices[0].x(), fmin(vertices[1].x(), vertices[2].x())), fmin(vertices[0].y(), fmin(vertices[1].y(), vertices[2].y())), fmin(vertices[0].z(), fmin(vertices[1].z(), vertices[2].z())));
		Vector3f max_point(fmax(vertices[0].x(), fmax(vertices[1].x(), vertices[2].x())), fmax(vertices[0].y(), fmax(vertices[1].y(), vertices[2].y())), fmax(vertices[0].z(), fmax(vertices[1].z(), vertices[2].z())));
		box = BoundingBox(min_point - 0.01f, max_point + 0.01f);
		if(Vector3f::cross(this->normal.normalized(), Vector3f::UP).length() < eps)
            this->vDir = Vector3f::FORWARD;
        else
            this->vDir = Vector3f::UP;
        this->uDir = Vector3f::cross(this->normal.normalized(), this->vDir).normalized();
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
        Vector3f rd = ray.getDirection();
		Vector3f e1 = vertices[0] - vertices[1];
		Vector3f e2 = vertices[0] - vertices[2];
		Vector3f s = vertices[0] - ray.getOrigin();
		float saveDet = Matrix3f(rd,e1,e2).determinant();
		if(fabs(saveDet) < eps)
			return false;
		float actualT = Matrix3f(s,e1,e2).determinant()/saveDet;
		float beta = Matrix3f(rd,s,e2).determinant()/saveDet;
		float gamma = Matrix3f(rd,e1,s).determinant()/saveDet;
		if(actualT < tmin || beta < 0 || gamma < 0 || beta > 1 || gamma > 1 || beta + gamma > 1 || actualT > hit.getT())
			return false;
		hit.set(actualT, 0, 0, material, normal, ray);
		return true;
	}
	Vector3f normal;
	Vector3f vertices[3];

    void drawGL() override {
        Object3D::drawGL();
        glBegin(GL_TRIANGLES);
        glNormal3fv(normal);
        glVertex3fv(vertices[0]); glVertex3fv(vertices[1]); glVertex3fv(vertices[2]);
        glEnd();
    }

	bool getBoundingBox(BoundingBox &box) override
	{
		box = this->box;
		return true;
	}

protected:
};

#endif //TRIANGLE_H
