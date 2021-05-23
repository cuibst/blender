#include "plane.hpp"

class BoundingBox : public Object3D {

    Plane slab[3][2];

public:
    explicit BoundingBox(float lowX, float highX, float lowY, float highY, float lowZ, float highZ): Object3D() {
        slab[0][0] = Plane(Vector3f(-1,0,0), -lowX, nullptr);
        slab[0][1] = Plane(Vector3f(1,0,0), highX, nullptr);
        slab[1][0] = Plane(Vector3f(0,-1,0), -lowY, nullptr);
        slab[1][1] = Plane(Vector3f(0,1,0), highY, nullptr);
        slab[2][0] = Plane(Vector3f(0,0,-1), -lowZ, nullptr);
        slab[2][1] = Plane(Vector3f(0,0,1), highZ, nullptr);
        // std::cout<<"Bounding box: "<<lowX<<" "<<lowY<<" "<<lowZ<<" "<<highX<<" "<<highY<<" "<<highZ<<std::endl;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        float mn = -1e38, mx = 1e38;
        for(int i=0;i<3;i++)
        {
            Hit hitmin, hitmax;
            bool flag1, flag2;
            flag1 = slab[i][0].intersect(r, hitmin, tmin);
            flag2 = slab[i][1].intersect(r, hitmax, tmin);
            float t1 = hitmin.getT(), t2 = hitmax.getT();
            // cout << "t1=" << t1 << " t2=" << t2 << std::endl;
            if(!flag1 && !flag2)
                continue;
            else if(!flag1)
            {
                mx = min(mx, t2);
                continue;
            }
            else if(!flag2)
            {
                mx = min(mx, t1);
                continue;
            }
            if(t1 > t2)
                swap(t1, t2);
            mn = max(t1, mn);
            mx = min(t2, mx);
        }
        // std::cout<<mn<<" "<<mx<<std::endl;
        if(mn > mx)
            return false;
        if(mn < tmin)
            return false;
        h.set(mn, nullptr, Vector3f(0,0,0));
        return true;
    }
};