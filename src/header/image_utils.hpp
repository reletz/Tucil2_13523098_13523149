#ifndef _IMAGE_UTILS_HPP
#define _IMAGE_UTILS_HPP

#include <iostream>
#include <vector>
#include <cmath>

#include "FreeImagePlus.h"
#include "quadtree.hpp"
#include "io.hpp"

using namespace std;

namespace ImageUtils{
    vector<vector<RGB>> imageToMatrix(const string& filename);
    void matrixToImage(const vector<vector<RGB>>& image, const string& filename);
    
    void fillCompressedImage(QuadTreeNode* node, vector<vector<RGB>>& compressedImage);

    QuadTree compressImage(IO streams);
}

#endif 