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
        bool flag1 = false, flag2 = false;
        mn=mx=1e38;
        if(fabs(r.getDirection()[0]) > eps)
        {
            float t0 = (x[0] - r.getOrigin()[0]) / r.getDirection()[0];
            if(t0 > tmin)
            {
                Vector3f possible = r.pointAtParameter(t0);
                if(possible[1] > y[0] && possible[1] < y[1] && possible[2] > z[0] && possible[2] < z[1])
                {
                    if(!flag1)
                    {
                        flag1 = true;
                        mn = t0;
                    }
                    else
                    {
                        flag2 = true;
                        mx = t0;
                    }
                }
            }
            t0 = (x[1] - r.getOrigin()[0]) / r.getDirection()[0];
            if(t0 > tmin)
            {
                Vector3f possible = r.pointAtParameter(t0);
                if(possible[1] > y[0] && possible[1] < y[1] && possible[2] > z[0] && possible[2] < z[1])
                {
                    if(!flag1)
                    {
                        flag1 = true;
                        mn = t0;
                    }
                    else
                    {
                        flag2 = true;
                        mx = t0;
                    }
                }
            }
        }
        if(fabs(r.getDirection()[1]) > eps)
        {
            float t0 = (y[0] - r.getOrigin()[1]) / r.getDirection()[1];
            if(t0 > tmin)
            {
                Vector3f possible = r.pointAtParameter(t0);
                if(possible[0] > x[0] && possible[0] < x[1] && possible[2] > z[0] && possible[2] < z[1])
                {
                    if(!flag1)
                    {
                        flag1 = true;
                        mn = t0;
                    }
                    else
                    {
                        flag2 = true;
                        mx = t0;
                    }
                }
            }
            t0 = (y[1] - r.getOrigin()[1]) / r.getDirection()[1];
            if(t0 > tmin)
            {
                Vector3f possible = r.pointAtParameter(t0);
                if(possible[0] > x[0] && possible[0] < x[1] && possible[2] > z[0] && possible[2] < z[1])
                {
                    if(!flag1)
                    {
                        flag1 = true;
                        mn = t0;
                    }
                    else
                    {
                        flag2 = true;
                        mx = t0;
                    }
                }
            }
        }
        if(fabs(r.getDirection()[2]) > eps)
        {
            float t0 = (z[0] - r.getOrigin()[2]) / r.getDirection()[2];
            if(t0 > tmin)
            {
                Vector3f possible = r.pointAtParameter(t0);
                if(possible[0] > x[0] && possible[0] < x[1] && possible[1] > y[0] && possible[1] < y[1])
                {
                    if(!flag1)
                    {
                        flag1 = true;
                        mn = t0;
                    }
                    else
                    {
                        flag2 = true;
                        mx = t0;
                    }
                }
            }
            t0 = (z[1] - r.getOrigin()[2]) / r.getDirection()[2];
            if(t0 > tmin)
            {
                Vector3f possible = r.pointAtParameter(t0);
                if(possible[0] > x[0] && possible[0] < x[1] && possible[1] > y[0] && possible[1] < y[1])
                {
                    if(!flag1)
                    {
                        flag1 = true;
                        mn = t0;
                    }
                    else
                    {
                        flag2 = true;
                        mx = t0;
                    }
                }
            }
        }
        if(mn > mx)
            swap(mn, mx);
        return flag1 || flag2;
    }

    static BoundingBox mergeBox(const BoundingBox &a, const BoundingBox &b)
    {
        return BoundingBox(min(a.x[0], b.x[0]), max(a.x[1], b.x[1]), min(a.y[0], b.y[0]), max(a.y[1], b.y[1]), min(a.z[0], b.z[0]), max(a.z[1], b.z[1]));
    }
};