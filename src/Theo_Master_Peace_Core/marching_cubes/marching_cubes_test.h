#pragma once
#include "marching_cubes/marching_cubes.h"

marching_cubes::Mesh marching_cubes_test(){
    marching_cubes::MarchingCubesChunk mct = marching_cubes::MarchingCubesChunk(3,3,3,1);
    mct.setDensity(1,1,1,2);
    return marchingCubesToMesh(mct);
}
std::map<unsigned int, marching_cubes::Mesh> marching_cubes_test2(){

    marching_cubes::MarchingCubesChunk mct = marching_cubes::MarchingCubesChunk(3,3,3,1);

    mct.setDensity(1,1,1,2);
    mct.setType(1,1,1,0);
    
    return marchingCubesToMeshWithTypes(mct);
}