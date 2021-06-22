#ifndef CURVE_HPP
#define CURVE_HPP

#include "object3d.hpp"
#include <vecmath.h>
#include <vector>
#include <utility>

#include <algorithm>

// TODO (PA3): Implement Bernstein class to compute spline basis function.
//       You may refer to the python-script for implementation.

// The CurvePoint object stores information about a point on a curve
// after it has been tesselated: the vertex (V) and the tangent (T)
// It is the responsiblility of functions that create these objects to fill in all the data.
struct CurvePoint {
    Vector3f V; // Vertex
    Vector3f T; // Tangent  (unit)
    float t;
};

class Curve : public Object3D {
protected:
    std::vector<Vector3f> controls;
public:
    explicit Curve(std::vector<Vector3f> points) : controls(std::move(points)) {}

    bool intersect(const Ray &r, Hit &h, float tmin, float T) override {
        return false;
    }

    std::vector<Vector3f> &getControls() {
        return controls;
    }

    virtual CurvePoint pointAtT(float t) = 0;

    virtual void discretize(int resolution, std::vector<CurvePoint>& data) = 0;

    void drawGL() override {
        Object3D::drawGL();
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING);
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        glPointSize(4);
        glBegin(GL_POINTS);
        for (auto & control : controls) { glVertex3fv(control); }
        glEnd();
        std::vector<CurvePoint> sampledPoints;
        discretize(30, sampledPoints);
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (auto & cp : sampledPoints) { glVertex3fv(cp.V); }
        glEnd();
        glPopAttrib();
    }

    virtual std::pair<float, float> definitionRegion() = 0;

    bool getBoundingBox(BoundingBox &box) override {return false;}
};

class BezierCurve : public Curve {

public:
    explicit BezierCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4 || points.size() % 3 != 1) {
            printf("Number of control points of BezierCurve must be 3n+1!\n");
            exit(0);
        }
        int n = controls.size() - 1;
        for(int _=0;_<NUM_THREADS;_++)
        {
            for(int i=0;i<=n;i++)
                b[_][i].resize(n+1);
            bp[_].resize(n+1);
        }
    }

    std::vector<float> b[NUM_THREADS][20];
    std::vector<float> bp[NUM_THREADS];

    CurvePoint pointAtT(float t) override{
        int n = controls.size() - 1;
        int _ = omp_get_thread_num();
        for(int i=0;i<=n;i++)
            for(int j=0;j<=n;j++)
                b[_][i][j] = 0;
        // float **b = new float*[n + 1];
        // for(int i=0;i<=n;i++)
        // {
        //     b[i] = new float[n+1];
        //     for(int j=0;j<=n;j++)
        //         b[i][j] = 0;
        // }
        // float *bp = new float[n+1];
        b[_][0][0] = 1;
        for(int i=1;i<=n;i++)
        {
            b[_][i][0] = b[_][i-1][0] * (1 - t);
            for(int j=1;j<=n;j++)
                b[_][i][j] = b[_][i-1][j] * (1 - t) + b[_][i-1][j-1] * t;
        }
        bp[_][0] = n * (-b[_][n-1][0]);
        for(int i=1;i<=n;i++)
            bp[_][i] = n * (b[_][n-1][i-1] - b[_][n-1][i]);
        CurvePoint ret;
        ret.T = Vector3f(0,0,0);
        ret.V = Vector3f(0,0,0);
        ret.t = t;
        for(int i=0;i<=n;i++)
        {
            ret.T += bp[_][i] * controls[i];
            ret.V += b[_][n][i] * controls[i];
        }
        // delete[] bp;
        // for(int i=0;i<=n;i++)
        //     delete[] b[i];
        // delete[] b;
        return ret;
    }

    std::pair<float, float> definitionRegion() override{
        return std::make_pair(0,1);
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // TODO (PA3): fill in data vector
        int vertexCount = (controls.size() - 1) * resolution + 1;
        float step = 1.0f / (vertexCount - 1);
        for(int _=0;_<vertexCount;_++)
        {
            float t = step * _;
            data.push_back(pointAtT(t));
        }
    }

    ~BezierCurve()
    {
    }

protected:

};

class BsplineCurve : public Curve {

    const static int k = 3;
    std::vector<float> t;

    std::vector<float> b[NUM_THREADS][4], bp[NUM_THREADS];

public:
    BsplineCurve(const std::vector<Vector3f> &points) : Curve(points) {
        if (points.size() < 4) {
            printf("Number of control points of BspineCurve must be more than 4!\n");
            exit(0);
        }
        t.resize(controls.size() + k + 1);
        for(int i=0;i<=controls.size()+k;i++)
            t[i] = 1.0 * i / (controls.size() + k);
        for(int _=0;_<NUM_THREADS;_++)
        {
            for(int i=0;i<4;i++)
                b[_][i].resize(controls.size() + k + 1);
            bp[_].resize(controls.size() + k + 1);
        }
    }

    CurvePoint pointAtT(float T) override {
        // double* b[4], *bp;
        // for(int i=0;i<4;i++)
        //     b[i] = new double[controls.size() + k + 1];
        // bp = new double[controls.size() + k + 1];
        int _ = omp_get_thread_num();
        for(int i=0;i<controls.size()+k;i++)
        {
            if(T >= t[i] && T < t[i+1])
                b[_][0][i] = 1;
            else
                b[_][0][i] = 0;
            b[_][1][i] = b[_][2][i] = b[_][3][i] = bp[_][i] = 0;
        }
        for(int p=1;p<=k;p++)
            for(int i=0;i+p+1<=controls.size()+k;i++)
                b[_][p][i] = (T - t[i]) / (t[i+p] - t[i]) * b[_][p-1][i] + (t[i+p+1] - T) / (t[i+p+1] - t[i+1]) * b[_][p-1][i+1];
        for(int i=0;i+k+1<=controls.size()+k;i++)
            bp[_][i] = k * (b[_][k-1][i] / (t[i+k] - t[i]) - b[_][k-1][i+1] / (t[i+k+1] - t[i+1]));
        CurvePoint res;
        res.T = Vector3f(0,0,0);
        res.V = Vector3f(0,0,0);
        for(int i=0;i<controls.size();i++)
        {
            res.V += b[_][k][i] * controls[i];
            res.T += bp[_][i] * controls[i];
        }
        res.t = T;
        // for(int i=0;i<4;i++)
        //     delete[] b[i];
        // delete[] bp;
        return res;
    }

    std::pair<float, float> definitionRegion() override {
        return std::make_pair(t[k], t[controls.size()]);
    }

    void discretize(int resolution, std::vector<CurvePoint>& data) override {
        data.clear();
        // TODO (PA3): fill in data vector
        int vertexCount = (controls.size() - k) * resolution + 1;
        float step = (t[controls.size()] - t[k]) / (vertexCount - 1);
        for(int _=0;_<vertexCount;_++)
        {
            float T = _ * step + t[k];
            data.push_back(pointAtT(T));
        }
    }

    ~BsplineCurve() {
    }

protected:

};

#endif // CURVE_HPP
