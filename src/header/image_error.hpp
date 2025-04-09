#ifndef _IMAGE_ERROR_HPP
#define _IMAGE_ERROR_HPP

#include <iostream>
#include <vector>
#include "quadtree.hpp"
#include <cmath>

using namespace std;

namespace ImageError {
  RGB mean(const vector<vector<RGB>>& image, int x, int y, int width, int height);
  float variance(const vector<vector<RGB>>& image, int x, int y, int width, int height);
  float mad(const vector<vector<RGB>>& image, int x, int y, int width, int height);
  float maxDiff(const vector<vector<RGB>>& image, int x, int y, int width, int height);
  float entropy(const vector<vector<RGB>>& image, int x, int y, int width, int height);

  float ssim_channel(const vector<vector<int>>& A,
    const vector<vector<int>>& B,
    int xA, int yA, int xB, int yB,
    int width, int height);
    
  float ssim(const vector<vector<RGB>>& imageA,
    const vector<vector<RGB>>& imageB,
    int xA, int yA,
    int xB, int yB,
    int width, int height);
}

#endif