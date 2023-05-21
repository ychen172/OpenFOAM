//This function detect whether a path already exist to prevent unintended overwrite
#include <sys/stat.h>
#include <string>
bool IsPathExist(const std::string& s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}