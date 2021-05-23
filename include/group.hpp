#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// TODO (PA2): Implement Group - copy from PA1
class Group : public Object3D {

private:
    std::vector<Object3D*> objects;

public:

    Group() {

    }

    explicit Group (int num_objects) {
        objects.resize(num_objects);
        std::cout<<"Resize "<<num_objects<<" "<<objects.size()<<std::endl;
    }

    ~Group() override {

    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool ret = false;
        for(Object3D* obj : objects) {
            ret = obj->intersect(r,h,tmin) || ret;
        }
        return ret;
    }

    void addObject(int index, Object3D *obj) {
        objects[index] = obj;
    }

    int getGroupSize() {
        return (int)objects.size();
    }

    void drawGL() override {
        for(unsigned i=0;i<objects.size();i++)
            objects[i]->drawGL();
    }

private:

};

#endif
	
