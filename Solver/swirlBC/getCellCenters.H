//This function get the cell centers and cell area for the given patches
#undef __STRICT_ANSI__
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <tuple>
#include "fvCFD.H"
#include "pisoControl.H"
std::tuple <std::vector<std::vector<double>>, std::vector<double>> getCellCenters(std::string surfaceName)
{
    //Some OpenFOAM libraries need to readin those dummy inputs
    char dummy   = 'd';
    char* dummy2 = &dummy;
    char** argv  = &dummy2;
    int argc     = 1;
    #include "setRootCaseLists.H"
    #include "createTime.H"
    #include "createMesh.H"
    //Get surface information
    label patchID = mesh.boundaryMesh().findPatchID(surfaceName);
    const Foam::fvsPatchField<Foam::Vector<double>>& faceCenterHub = mesh.Cf().boundaryField()[patchID]; // Return a corrdinate vector for each cell face
    const Foam::fvsPatchField<Foam::Vector<double>>& faceAreaHub = mesh.Sf().boundaryField()[patchID];// Return a area vector for each cell face
    std::vector<std::vector<double>> cellCenters;
    std::vector<double> addIn, cellAreas;
    for (int indF = 0; indF < faceCenterHub.size(); indF++)
    {
        addIn = {faceCenterHub[indF][0], faceCenterHub[indF][1], faceCenterHub[indF][2]};
        cellCenters.push_back(addIn);
        cellAreas.push_back(std::pow(std::pow(faceAreaHub[indF][0],2)+std::pow(faceAreaHub[indF][1],2)+std::pow(faceAreaHub[indF][2],2),0.5));
    }
    return std::make_tuple(cellCenters,cellAreas);
}

/*
Fun Facts
for the __STRICT_ANSI__ library
Apparently, MinGW defines the Bessel functions only when __STRICT_ANSI__ is not defined
, and it is defined when -std=c++11 is specified. I was able to get your code to compile
 in MinGW by adding #undef __STRICT_ANSI__ at the top of the file. (for cmath I am not sure whether
 this can cause other side effect)
*/
