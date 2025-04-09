#ifndef _IO_H
#define _IO_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include "quadtree.hpp"

using namespace std;
namespace fs = std::filesystem;

class IO{
  public:
    fs::path imageSrcPath;
    double VAR_THRESHOLD = 0;
    long long MIN_BLOCK_SIZE = 0;
    int method = -1;
    fs::path imageDestPath;

    bool inputSrc(char const *argv[]);
    bool inputThreshold();
    bool inputMinBlock();
    bool inputMethod();
    bool inputDest(char const *argv[]);

    bool validYN(string msg);
  
    bool initInput(int argc, char const *argv[]);    
};

#endif