#pragma once

#include "hit.hpp"
#include "utilities.hpp"

#include <vector>
#include <algorithm>

class Node {
    Hit *hit;
    float maxRadius;
    Vector3f minVertex,maxVertex;

public:
    Node *lch, *rch;

    Node()
    {
        lch = rch = nullptr;
        hit = nullptr;
        maxRadius = 0;
        minVertex = Vector3f(1e38);
        maxVertex = Vector3f(-1e38);
    }

    Node(std::vector<Hit*> &hitList, int l, int r, int depth)
    {
        int axis = depth % 3;
        minVertex = Vector3f(1e38);
        maxVertex = Vector3f(-1e38);

        lch = rch = nullptr;
        maxRadius = 0;
        for(int i=l;i<=r;i++)
        {
            minVertex = min(minVertex, hitList[i]->point);
            maxVertex = max(maxVertex, hitList[i]->point);
            maxRadius = max(maxRadius, hitList[i]->radius);
        }
        if(l == r)
        {
            hit = hitList[l];
            return;
        }

        int mid = (l + r) >> 1;
        if(axis == 0)
            std::nth_element(hitList.begin() + l, hitList.begin() + mid, hitList.begin() + r + 1, [](Hit *x, Hit *y){
                return x->point.x() < y->point.x();
            });
        else if(axis == 1)
            std::nth_element(hitList.begin() + l, hitList.begin() + mid, hitList.begin() + r + 1, [](Hit *x, Hit *y){
                return x->point.y() < y->point.y();
            });
        else if(axis == 2)
            std::nth_element(hitList.begin() + l, hitList.begin() + mid, hitList.begin() + r + 1, [](Hit *x, Hit *y){
                return x->point.z() < y->point.z();
            });
        hit = hitList[mid];
        if(l < mid)
            lch = new Node(hitList, l, mid-1, depth+1);
        if(r > mid)
            rch = new Node(hitList, mid+1, r, depth+1);
    }

    void update(const Vector3f &photon, const Vector3f &attenuation, bool front)
    {
        float ret = 0;
        if (photon.x() > maxVertex.x()) ret += sqr(photon.x() - maxVertex.x());
        if (photon.x() < minVertex.x()) ret += sqr(minVertex.x() - photon.x());
        if (photon.y() > maxVertex.y()) ret += sqr(photon.y() - maxVertex.y());
        if (photon.y() < minVertex.y()) ret += sqr(minVertex.y() - photon.y());
        if (photon.z() > maxVertex.z()) ret += sqr(photon.z() - maxVertex.z());
        if (photon.z() < minVertex.z()) ret += sqr(minVertex.z() - photon.z());
        if(ret > maxRadius)
            return;
        if((hit->point - photon).squaredLength() <= hit->radius && hit->IsFrontFace() == front)
        {
            float disc = (hit->photonCount * DISCOUNT_COEFFICIENT + DISCOUNT_COEFFICIENT) / (hit->photonCount * DISCOUNT_COEFFICIENT + 1.0f);
            hit->photonCount ++;
            hit->radius *= sqrt(disc);
            hit->photonFlux = (hit->photonFlux + hit->attenuation * attenuation) * disc;
        }
        if(lch)
            lch->update(photon, attenuation, front);
        if(rch)
            rch->update(photon, attenuation, front);
        maxRadius = hit->radius;
        if(lch)
            maxRadius = max(lch->maxRadius, maxRadius);
        if(rch)
            maxRadius = max(rch->maxRadius, maxRadius);
    }
};