#ifndef _IMAGE_UTILS_HPP
#define _IMAGE_UTILS_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include "quadtree.hpp"

using namespace std;

namespace ImageUtils{
    vector<vector<RGB>> imageToMatrix(const string& filename);
    void matrixToImage(const vector<vector<RGB>>& image, const string& filename);
    void printMatrix(const vector<vector<RGB>>& image);
    vector<vector<RGB>> resize(const vector<vector<RGB>>& image, int newWidth, int newHeight);
    vector<vector<RGB>> normalize(const vector<vector<RGB>>& image, RGB mean);
}

#endif 