#ifndef REVSURFACE_HPP
#define REVSURFACE_HPP

#include "object3d.hpp"
#include "curve.hpp"
#include "bounding_box.hpp"
#include "sphere.hpp"
#include "mesh.hpp"
#include <tuple>
#include <math.h>

class RevSurface : public Object3D {

    Curve *pCurve;

    float ymin = 1e38, ymax = -1e38, xmax = 0;

    BoundingBox *box;

    std::pair<float, float> dRegion;

public:
    RevSurface(Curve *pCurve, Material* material) : pCurve(pCurve), Object3D(material) {
        // Check flat.
        for (const auto &cp : pCurve->getControls()) {
            if (cp.z() != 0.0) {
                printf("Profile of revSurface must be flat on xy plane.\n");
                exit(0);
            }
            ymin = min(ymin, cp.y());
            ymax = max(ymax, cp.y());
            xmax = max(xmax, fabs(cp.x()));
        }
        dRegion = pCurve->definitionRegion();
        box = new BoundingBox(-xmax, xmax, ymin - 2, ymax + 2, -xmax, xmax);
    }

    ~RevSurface() override {
        delete pCurve;
    }

    bool tryIntersect(const Ray &r, Hit &h, float tmin, double Tbegin, double tbegin, double fbegin)
    {
        Vector3f x = Vector3f(Tbegin, tbegin, fbegin);
        CurvePoint p;
        Vector3f dSdt, dSdf, F;
        Matrix3f dF;
        for(int _=0;_<=20;_++)
        {
            p = pCurve->pointAtT(x.y());
            dSdt = Vector3f(cos(x.z()) * p.T.x(), p.T.y(), sin(x.z()) * p.T.x());
            dSdf = Vector3f(-sin(x.z()) * p.V.x(), 0, cos(x.z()) * p.V.x());
            dF = Matrix3f(r.getDirection(), -dSdt, -dSdf);
            F = r.pointAtParameter(x.x()) - Vector3f(cos(x.z()) * p.V.x(), p.V.y(), sin(x.z()) * p.V.x());
            if(fabs(dF.determinant()) < eps || F.length() < 1e-3)
                break;
            x = x - dF.inverse() * F;
            x.z() = fmod(x.z(), 2 * PI);
            // while(x.z() < -PI)
            //     x.z() += 2 * PI;
            // while(x.z() > PI)
            //     x.z() -= 2 * PI;
        }
        if(F.length() > 1e-3)
            return false;
        p = pCurve->pointAtT(x.y());
        F = r.pointAtParameter(x.x()) - Vector3f(cos(x.z()) * p.V.x(), p.V.y(), sin(x.z()) * p.V.x());
        
        if(F.length() > 1e-3 || x.y() < dRegion.first || x.y() > dRegion.second || x.x() < tmin || x.x() > h.getT())
            return false;
        dSdt = Vector3f(cos(x.z()) * p.T.x(), p.T.y(), sin(x.z()) * p.T.x());
        dSdf = Vector3f(-sin(x.z()) * p.V.x(), 0, cos(x.z()) * p.V.x());
        Vector3f norm = -Vector3f::cross(dSdt, dSdf);
        norm.normalize();
        if(Vector3f::dot(norm, r.getDirection()) >= 0)
            norm.negate();
        h.set(x.x(), 0, 0, this->material, norm, r);
        return true;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        // (PA3 optional TODO): implement this for the ray-tracing routine using G-N iteration.
        float intermin, intermax;
        if(!box->intersect(r, intermin, intermax, tmin))
            return false;
        double step = (dRegion.second - dRegion.first) / 10.0;
        BoundingBox tmp(0,0,0,0,0,0);
        bool flag = false;
        double lt,rt;
        CurvePoint L,R;
        float ylow,yhigh;
        for(int i=0;i<10;i++)
        {
            lt = dRegion.first + step * i, rt = dRegion.first + step * (i + 1);
            L = pCurve->pointAtT(lt), R = pCurve->pointAtT(rt);
            ylow = min(L.V.y(), R.V.y()), yhigh = max(L.V.y(), R.V.y());
            
            tmp = BoundingBox(-xmax, xmax, ylow - 0.05, yhigh + 0.05, -xmax, xmax);
            if(tmp.intersect(r, intermin, intermax, tmin))
            {
                if(intermin > tmin)
                {
                    float phi = atan2(-r.pointAtParameter(intermin).z(), -r.pointAtParameter(intermin).x());
                    flag = tryIntersect(r, h, tmin, intermin, lt, phi) || flag;
                    flag = tryIntersect(r, h, tmin, intermin, (lt + rt) / 2, phi) || flag;
                }
                if(intermax < 1e30)
                {
                    float phi = atan2(-r.pointAtParameter(intermax).z(), -r.pointAtParameter(intermax).x());
                    flag = tryIntersect(r, h, tmin, intermax, lt, phi) || flag;
                    flag = tryIntersect(r, h, tmin, intermax, (lt + rt) / 2, phi) || flag;
                }
            }
        }
        if(!flag)
            return false;
        return true;

        // if(fabs(r.getDirection().y()) < 1e-3)
        // {
        //     std::cout << "undone" << std::endl;
        //     return false;
        // }
        // float t = savet;
        // Vector3f dir = r.getDirection(), ori = r.getOrigin();
        // float A = (dir.x() * dir.x() + dir.z() * dir.z()) / (dir.y() * dir.y());
        // float B = 2 * (dir.x() * ori.x() + dir.z() * ori.z()) / dir.y();
        // float C = (ori.x() * ori.x() + ori.z() * ori.z());
        // for(int _=0;_<=50;_++)
        // {
        //     CurvePoint p = pCurve->pointAtT(t);
        //     float F = A * p.V.y() * p.V.y() + B * p.V.y() + C - p.V.x() * p.V.x();
        //     float dF = A * p.T.y() * 2 * p.V.y() + B * p.T.y() - p.T.x() * 2 * p.V.x();
        //     if(fabs(dF) < eps || fabs(F) < eps)
        //         break;
        //     t = t - F / dF;
        // }
        // CurvePoint p = pCurve->pointAtT(t);
        // float F = A * p.V.y() * p.V.y() + B * p.V.y() + C - p.V.x() * p.V.x();
        // std::cout << F << " " << t << std::endl;
        // if(fabs(F) > 1e-3 || t > dRegion.second || t < dRegion.first)
        // {
        //     std::cout << "failed" << std::endl;
        //     return false;
        // }
        // float tr = (p.V.y() - ori.y()) / dir.y();
        // if(tr < tmin || tr > h.getT())
        //     return false;
        // Vector3f point = r.pointAtParameter(tr);
        // float theta = PI - atan2(point.z(), point.x());
        // Quat4f rot;
        // rot.setAxisAngle(theta, Vector3f::UP);
        // Vector3f pNew = Matrix3f::rotation(rot) * p.V;
        // Vector3f pNormal = Matrix3f::rotation(rot)* Vector3f::cross(p.T, -Vector3f::FORWARD);
        // std::cout << point << " " << pNew << std::endl;
        // h.set(tr, this->material, pNormal);
        // return true;

        

    }

    void drawGL() override {
        Object3D::drawGL();

        // Definition for drawable surface.
        typedef std::tuple<unsigned, unsigned, unsigned> Tup3u;
        // Surface is just a struct that contains vertices, normals, and
        // faces.  VV[i] is the position of vertex i, and VN[i] is the normal
        // of vertex i.  A face is a triple i,j,k corresponding to a triangle
        // with (vertex i, normal i), (vertex j, normal j), ...
        // Currently this struct is computed every time when canvas refreshes.
        // You can store this as member function to accelerate rendering.

        struct Surface {
            std::vector<Vector3f> VV;
            std::vector<Vector3f> VN;
            std::vector<Tup3u> VF;
        } surface;

        std::vector<CurvePoint> curvePoints;
        pCurve->discretize(30, curvePoints);
        const int steps = 40;
        for (unsigned int ci = 0; ci < curvePoints.size(); ++ci) {
            const CurvePoint &cp = curvePoints[ci];
            for (unsigned int i = 0; i < steps; ++i) {
                float t = (float) i / steps;
                Quat4f rot;
                rot.setAxisAngle(t * 2 * 3.14159, Vector3f::UP);
                Vector3f pnew = Matrix3f::rotation(rot) * cp.V;
                Vector3f pNormal = Vector3f::cross(cp.T, -Vector3f::FORWARD);
                Vector3f nnew = Matrix3f::rotation(rot) * pNormal;
                surface.VV.push_back(pnew);
                surface.VN.push_back(nnew);
                int i1 = (i + 1 == steps) ? 0 : i + 1;
                if (ci != curvePoints.size() - 1) {
                    surface.VF.emplace_back((ci + 1) * steps + i, ci * steps + i1, ci * steps + i);
                    surface.VF.emplace_back((ci + 1) * steps + i, (ci + 1) * steps + i1, ci * steps + i1);
                }
            }
        }

        glBegin(GL_TRIANGLES);
        for (unsigned i = 0; i < surface.VF.size(); i++) {
            glNormal3fv(surface.VN[std::get<0>(surface.VF[i])]);
            glVertex3fv(surface.VV[std::get<0>(surface.VF[i])]);
            glNormal3fv(surface.VN[std::get<1>(surface.VF[i])]);
            glVertex3fv(surface.VV[std::get<1>(surface.VF[i])]);
            glNormal3fv(surface.VN[std::get<2>(surface.VF[i])]);
            glVertex3fv(surface.VV[std::get<2>(surface.VF[i])]);
        }
        glEnd();
    }
};

#endif //REVSURFACE_HPP