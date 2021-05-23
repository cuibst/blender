#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "utilities.hpp"
#include <vecmath.h>
#include <vecio.h>
#include <float.h>
#include <cmath>
#include <glut.h>


class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up);
		this->horizontal.normalize();
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual void setupGLMatrix() {
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        gluLookAt(center.x(), center.y(), center.z(),   // Position
                  center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),   // LookAt
                  up.x(), up.y(), up.z());    // Up direction
    }

    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setCenter(const Vector3f& pos) {
        this->center = pos;
    }
    Vector3f getCenter() const {
        return this->center;
    }

    void setRotation(const Matrix3f& mat) {
        this->horizontal = mat.getCol(0);
        this->up = -mat.getCol(1);
        this->direction = mat.getCol(2);
    }
    Matrix3f getRotation() const {
        return Matrix3f(this->horizontal, -this->up, this->direction);
    }

    virtual void resize(int w, int h) {
        width = w; height = h;
    }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

class PerspectiveCamera : public Camera {

public:

    float getFovy() const { return fovyd; }

    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
            const Vector3f &up, int imgW, int imgH, float angle, float _aperture = 0, float _focalLength = 1) : Camera(center, direction, up, imgW, imgH) {
        // angle is in radian.
        this->angle = angle;    
        this->aperture = _aperture;
        this->focalLength = _focalLength;
        float h = tan(angle / 2.0);
        float w = h * (float(imgW) / float(imgH));
        fovyd = angle / 3.1415 * 180.0;
        fx = fy = (float) height / (2 * tanf(angle / 2));
        cx = width / 2.0f;
        cy = height / 2.0f;
    }

    Ray generateRay(const Vector2f &point) override {
        float csx = focalLength * (point.x() - cx) / fx;
        float csy = focalLength * (point.y() - cy) / fy;
        float dx = (2 * RAND() - 1) * aperture, dy = (2 * RAND() - 1) * aperture;
        Vector3f dir(csx - dx, -csy - dy, focalLength);
        Matrix3f R(horizontal, -up, direction);
        dir = (R * dir).normalized();
        Ray ray(center + horizontal * dx - up * dy, dir);
        return ray;
    }

    void resize(int w, int h) override {
        fx *= (float) h / height;
        fy = fx;
        Camera::resize(w, h);
        cx = width / 2.0f;
        cy = height / 2.0f;
    }

    void setupGLMatrix() override {
        // Extrinsic.
        Camera::setupGLMatrix();
        // Perspective Intrinsic.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // field of view in Y, aspect ratio, near crop and far crop.
        gluPerspective(fovyd, cx / cy, 0.01, 100.0);
    }

protected:
    float angle;

    //for depth of field
    float aperture, focalLength;

    // Perspective intrinsics
    float fx;
    float fy;
    float cx;
    float cy;
    float fovyd;
};

#endif //CAMERA_H
