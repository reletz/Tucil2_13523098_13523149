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
  float dim = float(width * height);
  if (dim == 0) return 0.0f;

  RGB imgMean = mean(image, x, y, width, height);

  for (int j = y; j < y + height; j++){
    for (int i = x; i < x + width; i++){
      R += abs(image[j][i].r - imgMean.r);
      G += abs(image[j][i].g - imgMean.g);
      B += abs(image[j][i].b - imgMean.b);
    }
  }

  return (R + G + B) / (3.0f * dim);
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

float ImageError::ssim(const vector<vector<RGB>>& image, int x, int y, int width, int height) {
  // Constants to stabilize division with weak denominator
  const float C1 = (0.01f * 255) * (0.01f * 255);
  const float C2 = (0.03f * 255) * (0.03f * 255);
  
  // Get mean of the block
  RGB blockMean = mean(image, x, y, width, height);
  
  // Create a block with uniform color (the compressed version)
  vector<vector<RGB>> compressedBlock(height, vector<RGB>(width, blockMean));
  
  // Calculate SSIM for each channel
  float ssimR = calculateChannelSSIM(image, compressedBlock, x, y, width, height, 'r', C1, C2);
  float ssimG = calculateChannelSSIM(image, compressedBlock, x, y, width, height, 'g', C1, C2);
  float ssimB = calculateChannelSSIM(image, compressedBlock, x, y, width, height, 'b', C1, C2);
  
  // Equal weights for each channel
  float wR = 0.33f;
  float wG = 0.33f;
  float wB = 0.33f;
  
  float result = wR * ssimR + wG * ssimG + wB * ssimB;
  
  // Ensure the result is between 0 and 1
  return max(0.0f, min(1.0f, result));
}

float ImageError::calculateChannelSSIM(
  const vector<vector<RGB>>& original,
  const vector<vector<RGB>>& compressed,
  int x, int y, int width, int height,
  char channel, float C1, float C2
) {
  // Original block
  float μx = 0.0f;
  float σx2 = 0.0f;
  
  // Compressed block
  float μy = 0.0f;
  float σy2 = 0.0f;
  
  int pixelCount = width * height;
  
  //Means
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      float valX, valY;
      
      if (channel == 'r') {
        valX = original[j+y][i+x].r;
        valY = compressed[j][i].r;
      } else if (channel == 'g') {
        valX = original[j+y][i+x].g;
        valY = compressed[j][i].g;
      } else { // 'b'
        valX = original[j+y][i+x].b;
        valY = compressed[j][i].b;
      }
      
      μx += valX;
      μy += valY;
    }
  }
  
  μx /= pixelCount;
  μy /= pixelCount;
  
  // Variances
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      float valX, valY;
      
      if (channel == 'r') {
        valX = original[j+y][i+x].r;
        valY = compressed[j][i].r;
      } else if (channel == 'g') {
        valX = original[j+y][i+x].g;
        valY = compressed[j][i].g;
      } else { // 'b'
        valX = original[j+y][i+x].b;
        valY = compressed[j][i].b;
      }
      
      float dx = valX - μx;
      float dy = valY - μy;
      
      σx2 += dx * dx;
      σy2 += dy * dy;
    }
  }
  
  σx2 /= pixelCount;
  σy2 /= pixelCount;
  
  // cout << "σx2: " << σx2 << " σy2: " << σy2 << endl;
  // cout << "μx: " << μx << " μy: " << μy << endl;
  
  // (2μxμy+C1)(C2) / ((μx²+μy²+C1)(σx²+C2))
  float numerator = (2 * μx * μy + C1) * (C2);
  float denominator = (μx * μx + μy * μy + C1) * (σx2 * σx2 + C2);
  
  if (denominator < 0.0001f) return 0.8f;  // Avoid division by very small numbers
  float rawResult = numerator / denominator;
  return rawResult;
}