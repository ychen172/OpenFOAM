//This function overwrite a parameter file such as 0/U
//The over writting information needs to be terminated with a \n
//For example printOut("example","Inlet","Outlet","Hello\nWorld\n!!\n");
#include <iostream>
#include <fstream>
#include <string>
#include "isPathExist.H"
void printOut(std::string fileName, std::string nameStart, std::string nameEnd, std::string linesPrint)
{
std::string lineIn , firstSec , secondSec, backupName;
std::ifstream fileIn;
int indSta,indEnd;
bool first {true};
bool second {false};
bool over {false};
fileIn.open (fileName,std::ios::app);
std::ofstream fileOut;
fileOut.open("tempOut",std::ios::app);

while (std::getline(fileIn,lineIn))
{
    if (!over)
    {
        indSta = lineIn.find(nameStart);
        if (indSta>=0)
        {
            firstSec.append(lineIn);
            firstSec.append("\n");// This is here or else the test line itself will be swallowed
            first = false;
        }
        indEnd = lineIn.find(nameEnd);
        if (indEnd>=0)
        {
            second = true;
            over = true;
        } 
    }

    if (first & !second)
    {
        firstSec.append(lineIn);
        firstSec.append("\n");  
    } else if(!first & second)
    {
        secondSec.append(lineIn);
        secondSec.append("\n");  
    } else {}
}
fileIn.close();
fileOut << firstSec << linesPrint << secondSec;
fileOut.close();
//Prepare filenames for back up and the final one
const char* fileNameC = fileName.c_str();
backupName = "backup";
const char* backupNameC = backupName.c_str();
//Save files
if (IsPathExist(backupName))
{
    remove(fileNameC);
}
else
{
    rename(fileNameC,backupNameC);
}
rename("tempOut",fileNameC);
//Report Status
std::cout << "Finished Overwriting File: " << fileName << "\n";
std::cout << "Finished Creating File:    " << backupName << "\n"; 
return;
}

