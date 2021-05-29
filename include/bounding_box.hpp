#pragma once

class NOPlane {
public:
    NOPlane() {

    }

    NOPlane(const Vector3f &normal, float d, Material *m) {
        this->d = d;
        this->d /= normal.length();
        this->normal = normal.normalized();
        if(Vector3f::cross(this->normal.normalized(), Vector3f::UP).length() < eps)
            this->vDir = Vector3f::FORWARD;
        else
            this->vDir = Vector3f::UP;
        this->uDir = Vector3f::cross(this->normal.normalized(), this->vDir).normalized();
    }

    bool intersect(const Ray &r, Hit &h, float tmin) {
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
        h.set(actualT, u, v, nullptr, normal, r);
        return true;
    }

protected:
    Vector3f normal;
    float d;
    Vector3f uDir, vDir;

};

class BoundingBox {

    NOPlane slab[3][2];

public:
    float x[2], y[2], z[2];

    explicit BoundingBox() = default;

    explicit BoundingBox(float lowX, float highX, float lowY, float highY, float lowZ, float highZ) {
        slab[0][0] = NOPlane(Vector3f(-1,0,0), -lowX, nullptr);
        slab[0][1] = NOPlane(Vector3f(1,0,0), highX, nullptr);
        slab[1][0] = NOPlane(Vector3f(0,-1,0), -lowY, nullptr);
        slab[1][1] = NOPlane(Vector3f(0,1,0), highY, nullptr);
        slab[2][0] = NOPlane(Vector3f(0,0,-1), -lowZ, nullptr);
        slab[2][1] = NOPlane(Vector3f(0,0,1), highZ, nullptr);
        x[0] = lowX;
        y[0] = lowY;
        z[0] = lowZ;
        x[1] = highX;
        y[1] = highY;
        z[1] = highZ;
        // std::cout<<"Bounding box: "<<lowX<<" "<<lowY<<" "<<lowZ<<" "<<highX<<" "<<highY<<" "<<highZ<<std::endl;
    }

    BoundingBox(const Vector3f &a, const Vector3f &b) : BoundingBox(min(a.x(), b.x()), max(a.x(), b.x()), min(a.y(), b.y()), max(a.y(), b.y()), min(a.z(), b.z()), max(a.z(), b.z())) {}

    bool intersect(const Ray &r, float &mn, float &mx, float tmin) {
        mn = -1e38, mx = 1e38;
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
        if(mn > mx || mx < tmin)
            return false;
        return true;
    }

    static BoundingBox mergeBox(const BoundingBox &a, const BoundingBox &b)
    {
        return BoundingBox(min(a.x[0], b.x[0]), max(a.x[1], b.x[1]), min(a.y[0], b.y[0]), max(a.y[1], b.y[1]), min(a.z[0], b.z[0]), max(a.z[1], b.z[1]));
    }
};