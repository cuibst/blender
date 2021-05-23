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

public:
	Triangle() = delete;
        ///@param a b c are three vertex positions of the triangle

	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		vertices[0] = a;
		vertices[1] = b;
		vertices[2] = c;
		normal = Vector3f::cross(b-a, c-a).normalized();
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

protected:
};

#endif //TRIANGLE_H
