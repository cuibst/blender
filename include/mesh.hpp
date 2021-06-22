#ifndef MESH_H
#define MESH_H

#include <vector>
#include "object3d.hpp"
#include "triangle.hpp"
#include "Vector2f.h"
#include "Vector3f.h"
#include "bvh.hpp"


class Mesh : public Object3D {

public:
    Mesh(const char *filename, Material *m);

    struct TriangleIndex {
        TriangleIndex() {
            x[0] = 0; x[1] = 0; x[2] = 0;
        }
        int &operator[](const int i) { return x[i]; }
        // By Computer Graphics convention, counterclockwise winding is front face
        int x[3]{};
    };

    std::vector<Vector3f> v;
    std::vector<TriangleIndex> t;
    std::vector<Vector3f> n;
    bool intersect(const Ray &r, Hit &h, float tmin, float T) override;

    void drawGL() override {
        // TODO (PA2): Call drawGL for each individual triangle.
		for(TriangleIndex triangleIndex : t)
        {
            Triangle triangle(v[triangleIndex.x[0]], v[triangleIndex.x[1]], v[triangleIndex.x[2]], material);
            triangle.drawGL();
        }
    }

    bool getBoundingBox(BoundingBox &box) override
    {
        box = this->box;
        return true;
    }

private:

    // Normal can be used for light estimation
    void computeNormal();
    bvhNode *root;
    std::vector<Object3D*> triangleList;
    BoundingBox box;
};

#endif
