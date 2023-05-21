/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    swirlBC

Description
    This function set a swirl flow inlet velocity boundary conditions on any patch
    Function Usage: swirlBC rCritFrac swirlNum axialVel mainAxis patchStart patchEnd fileName
    Default Inputs: swirlBC   0.35      1        10       3        Inlet     Outlet    0/U 
\*---------------------------------------------------------------------------*/

#include <string>
#include <vector>
#include <tuple>
#include <iostream>
#include "printOut.H"
#include "getCellCenters.H"
#include "calcSwirlBC.H"
int main(int argc, char* argv[])
{
    //Read in parameters with optional parameters
    std::vector<string> argvUse = {"swirlBC","0.35","1","-10","3","Inlet","Outlet","0/U"};//Default parameters 
    for (int ind = 0; ind < argc; ind++)
    {
        argvUse[ind] = argv[ind];
    }
    double rCritFrac = std::stod(argvUse[1]);
    double swirlNumb = std::stod(argvUse[2]);
    double axialVel  = std::stod(argvUse[3]);
    double swirlAxi  = std::stod(argvUse[4]);
    std::string startWord (argvUse[5]);
    std::string endWord   (argvUse[6]);
    std::string fileName  (argvUse[7]);//Optional input starts from 1
    //Initialize Variables
    std::vector<double> desPara = {rCritFrac,swirlNumb,axialVel,swirlAxi};
    std::vector<std::vector<double>> faceCenter;
    std::vector<double> faceArea,xVel,yVel,zVel;  
    //Read Cell Centers
    std::tie(faceCenter,faceArea) = getCellCenters(startWord); //First is coordinates of face centers and second is vector of areas
    //Report Inputs
    std::cout << "Inputs Are:\n";
    std::cout << "Critical Radial Fraction: " << argvUse[1] << "\n";
    std::cout << "Maximum Swirl Number:     " << argvUse[2] << "\n";
    std::cout << "Constant Axial Velocity:  " << argvUse[3] << " m/s\n";
    std::cout << "Around Axis Number:       " << argvUse[4] << "\n";
    std::cout << "Change Patch:             " << argvUse[5] << "\n";
    std::cout << "Ends at Patch:            " << argvUse[6] << "\n";
    std::cout << "Overwrite File:           " << argvUse[7] << "\n";
    //Calcualte Swirl BC
    std::tie(xVel,yVel,zVel)      = calcSwirlBC(faceCenter, faceArea, desPara);
    //Print out the BC
    std::string printWords("{\ntype fixedValue;\nvalue nonuniform List<vector>\n");
    printWords.append(std::to_string(xVel.size()));
    printWords.append("\n(\n");
    for (double ind = 0; ind < xVel.size(); ind++)
    {
        printWords.append("(");
        printWords.append(std::to_string(xVel[ind]));
        printWords.append(" ");
        printWords.append(std::to_string(yVel[ind]));
        printWords.append(" ");
        printWords.append(std::to_string(zVel[ind]));
        printWords.append(")\n");
    }
    printWords.append(")\n;\n}\n");
    printOut(fileName, startWord, endWord, printWords);
    //Report Status
    std::cout << "End" << std::endl;
    return(0);
}

/*
Fun Facts
//This argc is the number of optional input and argv are the optional string input. This is for command line access to function.
//the first element of argv will always be the function call itself. so skip the first element if needed
//Also the argc start from 0 to i<argc to avoide index out of range
//Also for for loo, the first element is executed before the block. second element tested before the block, and the final elemnt after the block
*/
