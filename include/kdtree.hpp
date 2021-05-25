#pragma once

/**
 * This file has not been put into use!!
 */

#include "object3d.hpp"

#include <algorithm>

class Node : public Object3D {

    Object3D *lch, *rch;
    BoundingBox box;
    Object3D *obj;
    int axis;

public:
    Node()
    {
        lch = rch = nullptr;
    }

    Node(std::vector<Object3D*> &objects, int l, int r, int depth)
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
                std::sort(tmp.begin() + l, tmp.begin() + r + 1, cmpX);
            if(axis == 1)
                std::sort(tmp.begin() + l, tmp.begin() + r + 1, cmpY);
            if(axis == 2)
                std::sort(tmp.begin() + l, tmp.begin() + r + 1, cmpZ);
            obj = tmp[mid];
            tmp[mid]->getBoundingBox(box);
            if(l < mid)
                lch = new Node(tmp, l, mid - 1, depth + 1);
            if(r > mid)
                rch = new Node(tmp, mid + 1, r, depth + 1);
        }
        BoundingBox tmp;
        if(lch && lch->getBoundingBox(tmp))
            box = mergeBox(box, tmp);
        if(rch && rch->getBoundingBox(tmp))
            box = mergeBox(box, tmp);
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
        Hit tmpHit;
        if(!box.intersect(r, tmpHit, tmin))
            return false;
        bool sh = obj->intersect(r, h, tmin), lh = lch ? lch->intersect(r, h, tmin) : false, rh = rch ? rch->intersect(r, h, tmin) : false;
        return sh || (lh || rh);
    }

    bool getBoundingBox(BoundingBox &b) override
    {
        b = box;
        return true;
    }

    static bool cmpX(Object3D *a, Object3D *b)
    {
        BoundingBox aa, bb;
        if(!a->getBoundingBox(aa)||!b->getBoundingBox(bb))
            return false;
        return aa.minPoint().x() < bb.minPoint().x();
    }
    
    static bool cmpY(Object3D *a, Object3D *b)
    {
        BoundingBox aa, bb;
        if(!a->getBoundingBox(aa)||!b->getBoundingBox(bb))
            return false;
        return aa.minPoint().y() < bb.minPoint().y();
    }
    
    static bool cmpZ(Object3D *a, Object3D *b)
    {
        BoundingBox aa, bb;
        if(!a->getBoundingBox(aa)||!b->getBoundingBox(bb))
            return false;
        return aa.minPoint().z() < bb.minPoint().z();
    }
};