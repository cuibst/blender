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
# bin/PA3 testcases/RayCast/scene05_bunny_200.txt output/scene05-fine.bmp --raycast
# bin/PA3 testcases/RayCast/scene06_bunny_1k.txt output/scene06.bmp --raycast
# bin/PA3 testcases/RayCast/scene07_shine.txt output/scene07.bmp --raycast
# bin/PA3 testcases/RayCast/scene08_core.txt output/scene08.bmp --raycast
# bin/PA3 testcases/RayCast/scene09_norm.txt output/scene09.bmp --raycast
# bin/PA3 testcases/RayCast/scene10_wineglass.txt output/scene10.bmp --raycast

# Path trace testcases.
# bin/PA3 testcases/PathTrace/scene11_basic.txt output/scene11-pt.bmp --pathtrace
# bin/PA3 testcases/PathTrace/scene12_wineglass.txt output/scene12-pt.bmp --pathtrace
# bin/PA3 testcases/PathTrace/scene13_complex_objects.txt output/scene13-pt.bmp --pathtrace
# bin/PA3 testcases/PathTrace/scene14_texture.txt output/scene14-pt.bmp --pathtrace
# bin/PA3 testcases/PathTrace/scene15_vase.txt output/scene15-pt.bmp --pathtrace
# bin/PA3 testcases/PathTrace/scene16_bump.txt output/scene16-pt.bmp --pathtrace
# bin/PA3 testcases/PathTrace/scene17_aperture.txt output/scene17-pt.bmp --pathtrace
bin/PA3 testcases/PathTrace/scene19_move.txt output/scene19-pt.bmp --pathtrace

# SPPM testcases.
# bin/PA3 testcases/PathTrace/scene11_basic.txt output/scene11/scene11-sppm.bmp --sppm
# bin/PA3 testcases/PathTrace/scene12_wineglass.txt output/scene12/scene12-sppm.bmp --sppm
# bin/PA3 testcases/PathTrace/scene13_complex_objects.txt output/scene13/scene13-sppm.bmp --sppm
# bin/PA3 testcases/PathTrace/scene14_texture.txt output/scene14/scene14-sppm.bmp --sppm
# bin/PA3 testcases/PathTrace/scene15_vase.txt output/scene15/scene15-sppm.bmp --sppm
# bin/PA3 testcases/PathTrace/scene16_bump.txt output/scene16/scene16-sppm.bmp --sppm
# bin/PA3 testcases/PathTrace/scene17_aperture.txt output/scene17/scene17-sppm.bmp --sppm
# bin/PA3 testcases/PathTrace/scene18_final.txt output/scene18/scene18-sppm.bmp --sppm
bin/PA3 testcases/PathTrace/scene19_move.txt output/scene19/scene19-sppm.bmp --sppm