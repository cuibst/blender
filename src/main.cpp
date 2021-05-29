#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "renderer.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"

#include <string>

using namespace std;

#include "utilities.hpp"


int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    if (argc != 4) {
        cout << "Usage: ./bin/PA1 <input scene file> <output bmp file>" << endl;
        return 1;
    }
    string inputFile = argv[1];
    string outputFile = argv[2];  // only bmp is allowed.
    string colorizer = argv[3];

    std::cout << inputFile << " " << outputFile << std::endl;

    // TODO: Main RayCasting Logic
    // First, parse the scene using SceneParser.
    // Then loop over each pixel in the image, shooting a ray
    // through that pixel and finding its intersection with
    // the scene.  Write the color at the intersection to that
    // pixel in your output image.

    SceneParser parser = SceneParser(argv[1]);

    Renderer *renderer = nullptr;
    if(colorizer == "--raycast")
        renderer = new PathTraceRender(30, USE_RAY_CAST);
    else if(colorizer == "--pathtrace")
        renderer = new PathTraceRender(50, USE_PATH_TRACE);
    else if(colorizer == "--sppm")
        renderer = new SPPMRenderer(100, 1000000);
    renderer->render(parser, argv[2]);
    // cout << "Hello! Computer Graphics!" << endl;
    return 0;
}

