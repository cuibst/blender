#pragma once

#include "object3d.hpp"
#include <algorithm>

class bvhNode : public Object3D
{

    Object3D *lch, *rch;
    BoundingBox box;
    Object3D *obj;
    int axis;

public:
    bvhNode()
    {
        lch = rch = nullptr;
    }

    bvhNode(std::vector<Object3D*> &objects, int l, int r, int depth = 0)
    {
        axis = depth % 3;
        lch = rch = nullptr;

        if(l == r)
        {
            objects[l]->getBoundingBox(box);
            obj = objects[l];
            return;
        }

        if(l + 1 == r)
        {
            lch = objects[l + 1];
            obj = objects[l];
            objects[l]->getBoundingBox(box);
        }
        else if(l + 2 == r)
        {
            lch = objects[l + 1];
            rch = objects[l + 2];
            obj = objects[l];
            objects[l]->getBoundingBox(box);
        }
        else
        {
            std::vector<Object3D*> tmp = objects;
            int mid = (l + r) >> 1;
            if(axis == 0)
                std::sort(tmp.begin() + l, tmp.begin() + r + 1, [](Object3D *a, Object3D *b){
                    BoundingBox aa, bb;
                    if(!a->getBoundingBox(aa)||!b->getBoundingBox(bb))
                        return false;
                    return aa.x[0] < bb.x[0];
                });
            if(axis == 1)
                std::sort(tmp.begin() + l, tmp.begin() + r + 1, [](Object3D *a, Object3D *b){
                    BoundingBox aa, bb;
                    if(!a->getBoundingBox(aa)||!b->getBoundingBox(bb))
                        return false;
                    return aa.y[0] < bb.y[0];
                });
            if(axis == 2)
                std::sort(tmp.begin() + l, tmp.begin() + r + 1, [](Object3D *a, Object3D *b){
                    BoundingBox aa, bb;
                    if(!a->getBoundingBox(aa)||!b->getBoundingBox(bb))
                        return false;
                    return aa.z[0] < bb.z[0];
                });
            obj = tmp[mid];
            tmp[mid]->getBoundingBox(box);
            if(l < mid)
                lch = new bvhNode(tmp, l, mid - 1, depth + 1);
            if(r > mid)
                rch = new bvhNode(tmp, mid + 1, r, depth + 1);
        }
        BoundingBox tmp;
        if(lch && lch->getBoundingBox(tmp))
            box = BoundingBox::mergeBox(box, tmp);
        if(rch && rch->getBoundingBox(tmp))
            box = BoundingBox::mergeBox(box, tmp);
    }

    bool intersect(const Ray &r, Hit &h, float tmin, float T) override
    {
        // std::cout << "============" << std::endl;
        // std::cout << "ray is origin:" << r.getOrigin() << " direction:" << r.getDirection() << std::endl;
        // std::cout << "bounding box:" << box.x[0] << " " << box.y[0] << " " << box.z[0] << " " << box.x[1] << " " << box.y[1] << " " << box.z[1] << std::endl;
        float t_min, t_max;
        if(!box.intersect(r, t_min, t_max, tmin))
            return false;
        // std::cout << "intersected! mn:" << t_min << " " << " mx:" << t_max << std::endl;
        // std::cout << r.pointAtParameter(t_min) << " " << r.pointAtParameter(t_max) << std::endl;
        // std::cout << "============" << std::endl;
        bool sh = obj->intersect(r, h, tmin, T), lh = lch ? lch->intersect(r, h, tmin, T) : false, rh = rch ? rch->intersect(r, h, tmin, T) : false;
        return sh || (lh || rh);
    }

    bool getBoundingBox(BoundingBox &b) override
    {
        b = box;
        return true;
    }
};