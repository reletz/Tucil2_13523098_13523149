#ifndef _IMAGE_ERROR_HPP
#define _IMAGE_ERROR_HPP

#include <iostream>
#include <vector>
#include "quadtree.hpp"
#include <cmath>

using namespace std;

namespace ImageError {
  RGB mean(const std::vector<std::vector<RGB>>& image, int x, int y, int width, int height);
  float variance(const vector<vector<RGB>>& image, int x, int y, int width, int height);
  float mad(const vector<vector<RGB>>& image, int x, int y, int width, int height);
  float maxDiff(const vector<vector<RGB>>& image, int x, int y, int width, int height);
  float entropy(const vector<vector<RGB>>& image, int x, int y, int width, int height);
}

#endif