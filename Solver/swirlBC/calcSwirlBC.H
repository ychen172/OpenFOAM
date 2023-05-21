//This function calculate the swirl boundary conditions for the main function
#undef __STRICT_ANSI__ // See getCellCenters.H to understand why we need this
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <tuple>
std::tuple <std::vector<double>, std::vector<double>, std::vector<double>> calcSwirlBC(std::vector<std::vector<double>> cellCenter, std::vector<double> cellArea, std::vector<double> desPara)
{
    double rCritFrac, swirlNum, velAxi, Axi;
    rCritFrac = desPara[0]; // Critical location fraction of the two profiles
    swirlNum  = desPara[1]; // Maximum swirl number at the inlet
    velAxi    = desPara[2]; // Constant axial velocity m/s
    Axi       = desPara[3]; // Along which axis to apply the swirl flow
    //Assign the axis of rotation
    std::vector<double> xVec,yVec,zVec,rVec,thetaVec,velTanVec;
    std::vector<double>* fVec;
    std::vector<double>* sVec;
    std::vector<double>* tVec;
    if (Axi == 1) //x axis is the axis
    {
        fVec = &yVec; 
        sVec = &zVec;
        tVec = &xVec;
    }
    else if (Axi == 2) //y axis is the axis
    {
        fVec = &zVec;
        sVec = &xVec;
        tVec = &yVec;
    }
    else //z axis is the axis
    {
        fVec = &xVec;
        sVec = &yVec;
        tVec = &zVec;
    }
    // Calculate mean coordinates
    double xAve = 0,yAve = 0,zAve = 0;
    for (double ind = 0; ind < cellCenter.size(); ind++)
    {
        xAve += cellCenter[ind][0]*cellArea[ind]; // Area averaged coordinates
        yAve += cellCenter[ind][1]*cellArea[ind];
        zAve += cellCenter[ind][2]*cellArea[ind];
    }
    xAve /= cellCenter.size(); // Area averaged coordinates
    yAve /= cellCenter.size();
    zAve /= cellCenter.size();
    // Report Cell Centers
    std::cout << "Surface Center At:\n";
    std::cout << "X: " << std::to_string(xAve) << "\n";
    std::cout << "Y: " << std::to_string(yAve) << "\n";
    std::cout << "Z: " << std::to_string(zAve) << "\n";
    // Now we read out the true relative coordinates
    double rmax = 0;
    for (double ind = 0; ind < cellCenter.size(); ind++)
    {        
        xVec.push_back(cellCenter[ind][0]-xAve);
        yVec.push_back(cellCenter[ind][1]-yAve);
        zVec.push_back(cellCenter[ind][2]-zAve);
        rVec.push_back(std::pow(std::pow((*fVec)[ind],2.0)+std::pow((*sVec)[ind],2.0),0.5)); 
        rmax = (rmax < rVec[ind]) ? rVec[ind] : rmax;
        // Get the angle of each point (most confusing part)
        thetaVec.push_back(std::asin((*sVec)[ind]/rVec[ind]));
        if ((*fVec)[ind] < 0)
        {
            thetaVec[ind] = M_PI - thetaVec[ind]; // special treatement for 2nd and 3rd quadrans
        }
        thetaVec[ind] += (M_PI/2.); //This is the velocity angle converted from location angle
    }
    // Report Inlet Radius
    std::cout << "R: " << std::to_string(rmax) << "\n";
    // calculate the effective angular velocity based on the swirl number
    // Here I define my swirl number to be maximum tangential velocity / constant axial velocity
    double velTanMax, omega;
    velTanMax = swirlNum*velAxi;
    omega = velTanMax/(rCritFrac*rmax);
    // Calculate tangetial velocity and reassign back to xyz coordinate system
    // Overwrite the xyz coordinate by xyz velocity (No need to use another conditional test)
    for (double ind = 0; ind < cellCenter.size(); ind++)
    {
        if (rVec[ind]<(rCritFrac*rmax))
        {
            velTanVec.push_back(omega*rVec[ind]); //rigid body rotation
        } 
        else
        {
            velTanVec.push_back((omega*std::pow((rCritFrac*rmax),2.0))/rVec[ind]);
        }
        (*fVec)[ind] = velTanVec[ind] * std::cos(thetaVec[ind]);
        (*sVec)[ind] = velTanVec[ind] * std::sin(thetaVec[ind]);
        (*tVec)[ind] = velAxi;
    }
    return std::make_tuple(xVec,yVec,zVec);
}

/*
Fun Facts
This argc is the number of optional input and argv are the optional string input. This is for command line access to function.
the first element of argv will always be the function call itself. so skip the first element if needed
Also the argc start from 0 to i<argc to avoide index out of range
Also for for loo, the first element is executed before the block. second element tested before the block, and the final elemnt after the block
#include <iostream>

"Do variables declared in a conditional go out of scope at the end of the conditional?"
Yes - the scope of a local variable only falls within enclosing brackets:
so you cannot access fVec here
So here you want to initialize it before the condition statements
Because reference once initialized cannot be rebounded/reassigned later on
You need to use pointer in this case
Remember when later you use it, you need to expose it by (*fVec)[0]
*/

