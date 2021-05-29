#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "bvh.hpp"
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
        // if(root == nullptr)
        //     BuildTree();
        // return root->intersect(r, h, tmin);
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

    std::vector<Object3D*> getLightSources() const {
        std::vector<Object3D*> ret;
        for(auto obj : objects)
            if(obj->getMaterial() != nullptr && obj->getMaterial()->Emission() != Vector3f::ZERO)
                ret.push_back(obj);
        for(int i=0;i<objects.size();i++)
            if(objects[i]->getMaterial() == nullptr)
                std::cout << i << " material error!" << std::endl;
        return ret;
    }

    bool getBoundingBox(BoundingBox &box) override
    {
        bool flag = false;
        BoundingBox ret;
        for(auto obj : objects)
        {
            BoundingBox tmp;
            if(!obj->getBoundingBox(tmp))
                return false;
            if(!flag)
            {
                box = tmp;
                flag = true;
            }
            else
                box = BoundingBox::mergeBox(box, tmp);
        }
        return true;
    }

    void BuildTree()
    {
        root = new bvhNode(objects, 0, objects.size() - 1);
    }

private:
    bvhNode *root = nullptr;

};

#endif
	
