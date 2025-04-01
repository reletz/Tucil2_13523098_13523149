#include "header/image_error.hpp"

RGB ImageError::mean(const std::vector<std::vector<RGB>>& image, int x, int y, int width, int height){
  long long R = 0, G = 0, B = 0;
  int dim = width*height;

  for (int j = y; j < y + height; j++){
    for (int i = x; i < x + width; i++){
      R += image[j][i].r;
      G += image[j][i].g;
      B += image[j][i].b;
    }
  }

  RGB mean;
  mean.r = R / dim;
  mean.g = G / dim;
  mean.b = B / dim;

  return mean;
}

float ImageError::variance(const vector<vector<RGB>>& image, int x, int y, int width, int height){
  long long R = 0, G = 0, B = 0;
  int dr = 0, dg = 0, db = 0;
  float dim = float(width*height);
  RGB imgMean = mean(image, x, y, width, height);

  for (int j = y; j < y + height; j++){
    for (int i = x; i < x + width; i++){
      dr = image[j][i].r - imgMean.r;
      dg = image[j][i].g - imgMean.g;
      db = image[j][i].b - imgMean.b;
      R += dr*dr;
      G += dg*dg;
      B += db*db;
    }
  }

  return (R + G + B) / (3.0f*dim);
}

float ImageError::mad(const vector<vector<RGB>>& image, int x, int y, int width, int height){
  long long R = 0, G = 0, B = 0;
  float dim = float(width*height);
  RGB imgMean = mean(image, x, y, width, height);

  for (int j = y; j < y + height; j++){
    for (int i = x; i < x + width; i++){
      R = abs(image[j][i].r - imgMean.r);
      G = abs(image[j][i].g - imgMean.g);
      B = abs(image[j][i].b - imgMean.b);
    }
  }

  return (R + G + B) / (3.0f*dim);
}

float ImageError::maxDiff(const vector<vector<RGB>>& image, int x, int y, int width, int height) {
  int minR = 255, maxR = 0;
  int minG = 255, maxG = 0;
  int minB = 255, maxB = 0;

  for (int j = y; j < y + height; ++j) {
    for (int i = x; i < x + width; ++i) {
      const RGB& pixel = image[j][i];

      if (pixel.r < minR) minR = pixel.r;
      if (pixel.r > maxR) maxR = pixel.r;

      if (pixel.g < minG) minG = pixel.g;
      if (pixel.g > maxG) maxG = pixel.g;

      if (pixel.b < minB) minB = pixel.b;
      if (pixel.b > maxB) maxB = pixel.b;
    }
  }

  int dR = maxR - minR;
  int dG = maxG - minG;
  int dB = maxB - minB;

  return (dR + dG + dB) / 3.0f;
}


float ImageError::entropy(const vector<vector<RGB>>& image, int x, int y, int width, int height){
  return 0.0;
}