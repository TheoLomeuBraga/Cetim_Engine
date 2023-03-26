#pragma once
#include "marching_cubes/marching_cubes.h"

std::map<unsigned int, marching_cubes::Mesh> marching_cubes_test(){
    marching_cubes::MarchingCubesTerrain mct(3,3,3,1);
    mct.setDensity(1,1,1,1);
    return marchingCubesToMeshWithTypes(mct);
}