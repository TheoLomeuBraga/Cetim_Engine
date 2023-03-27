#pragma once
#include "marching_cubes/marching_cubes.h"

std::map<unsigned int, marching_cubes::Mesh> marching_cubes_test(){
    marching_cubes::MarchingCubesChunk mct(3,3,3,1);
    mct.setDensity(1,1,1,1);
    return marchingCubesToMeshWithTypes(mct);
}
std::map<unsigned int, marching_cubes::Mesh> marching_cubes_test2(){

    marching_cubes::MarchingCubesChunk mct(6,6,6,1);

    mct.setDensity(1,1,1,1);
    mct.setType(1,1,1,1);

    mct.setDensity(2,1,1,2);
    mct.setType(2,1,1,2);
    
    return marchingCubesToMeshWithTypes(mct);
}