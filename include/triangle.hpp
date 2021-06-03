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
	float d;

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
		this->d = Vector3f::dot(this->normal, a);
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
		Vector3f r_dir = ray.getDirection();

        if(abs(Vector3f::dot(r_dir.normalized(), this->normal.normalized())) < 1e-10)
            return false;

        float t = (this->d - Vector3f::dot(this->normal, ray.getOrigin())) / (Vector3f::dot(this->normal, ray.getDirection()));
        
		if (t < tmin || t > hit.getT() || isinf(t) || isnan(t) )
            return false;

		Vector3f inter_point = ray.pointAtParameter(t);
		for (int i = 0; i < 3; i ++){
			Vector3f line = inter_point - this->vertices[i];
			Vector3f line_a = this->vertices[(i+1)%3] - this->vertices[i];
			Vector3f line_b = this->vertices[(i+2)%3] - this->vertices[i];
			if (Vector3f::dot(Vector3f::cross(line, line_a), Vector3f::cross(line, line_b)) > 0)
				return false;
		}

		Vector3f hit_point = ray.pointAtParameter(t);
		float v = Vector3f::dot(hit_point, vDir);
        float u = Vector3f::dot(hit_point, uDir);
		Vector3f norm = this->normal;

        hit.set(t, u, v, this->material, norm, ray);

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
