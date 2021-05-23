#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 3) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.

    std::cout << inputFile << " " << outputFile << std::endl;

    // TODO: Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.

    SceneParser parser = SceneParser(argv[1]);
    Camera* camera = parser.getCamera();

    Image *img = new Image(parser.getCamera()->getWidth(),parser.getCamera()->getHeight());

    for(int x=0;x<camera->getWidth();x++)
        for(int y=0;y<camera->getHeight();y++)
        {
            Ray cameraRay = camera->generateRay(Vector2f(x,y));
            Group *group = parser.getGroup();
            Hit hit;
            bool isIntersect = group->intersect(cameraRay, hit, 0);
            if(isIntersect)
            {
                Vector3f finalColor = Vector3f::ZERO;
                for(int _=0;_<parser.getNumLights();_++)
                {
                    Light *light = parser.getLight(_);
                    Vector3f L, lightColor;
                    light->getIllumination(cameraRay.pointAtParameter(hit.getT()), L, lightColor);
                    finalColor += hit.getMaterial()->Shade(cameraRay, hit, L, lightColor);
                }
                img->SetPixel(x,y,finalColor);
            }
            else
                img->SetPixel(x,y,parser.getBackgroundColor());
        }
    img->SaveBMP(argv[2]);
    // cout << "Hello! Computer Graphics!" << endl;
    return 0;
}

