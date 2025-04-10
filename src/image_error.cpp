#include "header/image_error.hpp"
#include <cmath>
#include <vector>
#include <map>

using namespace std;

RGB ImageError::mean(const vector<vector<RGB>>& image, int x, int y, int width, int height){
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

float ImageError::ssim_channel(const vector<vector<int>>& A,
  const vector<vector<int>>& B,
  int xA, int yA, int xB, int yB,
  int width, int height) {

  /*  
  Rumus C: C = (K.L)^2.
  "
  Throughout this paper, the SSIM measure uses the following parameter settings: K1 = 0.01; K2 = 0.03.
  These values are somewhat arbitrary, but we find that in our current experiments, 
  the performance of the SSIM index algorithm is fairly insensitive to variations of these values
  "
  (Di Jurnal Halaman 7)
  */

  cerr << "[DEBUG] Masuk SSIM channel: " << xA << "," << yA << " size " << width << "x" << height << endl;

  if (width <= 1 || height <= 1) return 1.0f; 

  const float C1 = (0.01f * 255) * (0.01f * 255);
  const float C2 = (0.03f * 255) * (0.03f * 255);
  int N = width * height;

  if (N <= 1) return 1.0f;

  float meanA = 0.0f, meanB = 0.0f;
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      meanA += A[yA + j][xA + i];
      meanB += B[yB + j][xB + i];
    }
  }

  meanA /= N;
  meanB /= N;

  float varA = 0.0f, varB = 0.0f, covAB = 0.0f;
  for (int j = 0; j < height; ++j) {
    for (int i = 0; i < width; ++i) {
      float a = A[yA + j][xA + i] - meanA;
      float b = B[yB + j][xB + i] - meanB;
      varA += a * a;
      varB += b * b;
      covAB += a * b;
    }
  }
  varA /= N - 1;
  varB /= N - 1;
  covAB /= N - 1;

  cerr << "[DEBUG] VarA: " << varA << ", VarB: " << varB << ", CovAB: " << covAB << endl;


  float numerator = (2 * meanA * meanB + C1) * (2 * covAB + C2);
  float denominator = (meanA * meanA + meanB * meanB + C1) * (varA + varB + C2);
  cerr << "[DEBUG] Numerator: " << numerator << ", Denominator: " << denominator << endl;

  if (denominator == 0) return 1.0f;

  cerr << "[DEBUG] SSIM result: " << (numerator / denominator) << endl;
  return numerator / denominator;
}

float ImageError::ssim(const vector<vector<RGB>>& imageA,
  const vector<vector<RGB>>& imageB,
  int xA, int yA,
  int xB, int yB,
  int width, int height) {

  int H = imageA.size(), W = imageA[0].size();

  // Convert RGB to 3 channel matrix
  vector<vector<int>> channelAR(H, vector<int>(W));
  vector<vector<int>> channelAG(H, vector<int>(W));
  vector<vector<int>> channelAB(H, vector<int>(W));
  vector<vector<int>> channelBR(H, vector<int>(W));
  vector<vector<int>> channelBG(H, vector<int>(W));
  vector<vector<int>> channelBB(H, vector<int>(W));

  for (int j = 0; j < H; ++j) {
    for (int i = 0; i < W; ++i) {
      channelAR[j][i] = imageA[j][i].r;
      channelAG[j][i] = imageA[j][i].g;
      channelAB[j][i] = imageA[j][i].b;
      
      channelBR[j][i] = imageB[j][i].r;
      channelBG[j][i] = imageB[j][i].g;
      channelBB[j][i] = imageB[j][i].b;
    }
  }

  float ssimR = ssim_channel(channelAR, channelBR, xA, yA, xB, yB, width, height);
  float ssimG = ssim_channel(channelAG, channelBG, xA, yA, xB, yB, width, height);
  float ssimB = ssim_channel(channelAB, channelBB, xA, yA, xB, yB, width, height);

  return (ssimR + ssimG + ssimB) / 3.0f;
}