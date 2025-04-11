#include "header/image_error.hpp"
#include <map>

RGB ImageError::mean(const vector<vector<RGB>>& image, int x, int y, int width, int height) {
  if (width <= 0 || height <= 0) {
      return {0, 0, 0};
  }

  long sumR = 0, sumG = 0, sumB = 0;
  int count = 0;

  for (int j = y; j < y + height && j < image.size(); j++) {
      for (int i = x; i < x + width && i < image[j].size(); i++) {
          sumR += image[j][i].r;
          sumG += image[j][i].g;
          sumB += image[j][i].b;
          count++;
      }
  }

  if (count == 0) return {0, 0, 0};

  // Ensure we don't exceed RGB bounds (0-255)
  return {
      static_cast<uint8_t>(sumR / count),
      static_cast<uint8_t>(sumG / count),
      static_cast<uint8_t>(sumB / count)
  };
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


float ImageError::entropy(const vector<vector<RGB>>& image, int x, int y, int width, int height) {
  // histogram R, G, B
  map<int, int> histR, histG, histB;
  int totalPixels = width * height;

  // itung "tinggi" histogram
  for (int j = y; j < y + height; j++) {
    for (int i = x; i < x + width; i++) {
      int r = image[j][i].r;
      int g = image[j][i].g;
      int b = image[j][i].b;

      histR[r]++;
      histG[g]++;
      histB[b]++;
    }
  }

  // Entropi histogram
  auto calculateEntropy = [](const map<int, int>& hist, int totalPixels) -> float {
    float entropy = 0.0f;
    for (const auto& entry : hist) {
      float probability = float(entry.second) / totalPixels;
      if (probability > 0) {
        entropy -= probability * log2(probability);
      }
    } return entropy;
  };  

  float entropyR = calculateEntropy(histR, totalPixels);
  float entropyG = calculateEntropy(histG, totalPixels);
  float entropyB = calculateEntropy(histB, totalPixels);

  return (entropyR + entropyG + entropyB) / 3.0f;
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
  float meanx = 0.0f;
  float varx = 0.0f;
  
  // Compressed block
  float meany = 0.0f;
  float vary = 0.0f;
  
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
      
      meanx += valX;
      meany += valY;
    }
  }
  
  meanx /= pixelCount;
  meany /= pixelCount;
  
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
      
      float dx = valX - meanx;
      float dy = valY - meany;
      
      varx += dx * dx;
      vary += dy * dy;
    }
  }
  
  varx /= pixelCount;
  vary /= pixelCount;
  
  // cout << "σx2: " << σx2 << " σy2: " << σy2 << endl;
  // cout << "μx: " << μx << " μy: " << μy << endl;
  
  // (2μxμy+C1)(C2) / ((μx²+μy²+C1)(σx²+C2))
  float numerator = (2 * meanx * meany + C1) * (C2);
  float denominator = (meanx * meanx + meany * meany + C1) * (varx * varx + C2);
  
  if (denominator < 0.0001f) return 0.8f;  // Avoid division by very small numbers
  float rawResult = numerator / denominator;
  return rawResult;
}