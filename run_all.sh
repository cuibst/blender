#!/usr/bin/env bash

# If project not ready, generate cmake file.
if [[ ! -d build ]]; then
    mkdir -p build
    cd build
    cmake ..
    cd ..
else
    rm -r build
    mkdir -p build
    cd build
    cmake ..
    cd ..
fi

# Build project.
cd build
make -j
cd ..

# Run all testcases. 
# You can comment some lines to disable the run of specific examples.
# Ray cast testcases.
# bin/PA3 testcases/RayCast/scene01_basic.txt output/scene01.bmp --raycast
# bin/PA3 testcases/RayCast/scene02_cube.txt output/scene02.bmp --raycast
# bin/PA3 testcases/RayCast/scene03_sphere.txt output/scene03.bmp --raycast
# bin/PA3 testcases/RayCast/scene04_axes.txt output/scene04.bmp --raycast
# bin/PA3 testcases/RayCast/scene05_bunny_200.txt output/scene05.bmp --raycast
# bin/PA3 testcases/RayCast/scene06_bunny_1k.txt output/scene06.bmp --raycast
# bin/PA3 testcases/RayCast/scene07_shine.txt output/scene07.bmp --raycast
# bin/PA3 testcases/RayCast/scene08_core.txt output/scene08.bmp --raycast
# bin/PA3 testcases/RayCast/scene09_norm.txt output/scene09.bmp --raycast
# bin/PA3 testcases/RayCast/scene10_wineglass.txt output/scene10.bmp --raycast

# Path trace testcases.
# bin/PA3 testcases/PathTrace/scene11_basic.txt output/scene11.bmp --pathtrace
bin/PA3 testcases/PathTrace/scene12_wineglass.txt output/scene12.bmp --pathtrace