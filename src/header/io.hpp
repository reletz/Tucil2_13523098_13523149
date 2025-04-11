#ifndef _IO_H
#define _IO_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <limits>
#include "quadtree.hpp"

using namespace std;
namespace fs = std::filesystem;

class IO{
  public:
    fs::path imageSrcPath;
    float VAR_THRESHOLD = 0.0f;
    int MIN_BLOCK_SIZE = 0;
    int method = -1;
    fs::path imageDestPath;
    
    // Bonus features
    float TARGET_COMPRESSION = 0.0f;
    bool GENERATE_GIF = false;
    fs::path gifPath;

    bool inputSrc(char const *argv[]);
    bool inputThreshold();
    bool inputMinBlock();
    bool inputMethod();
    bool inputGifPath(char const *argv[]);
    bool inputDest(char const *argv[]);
    bool inputTargetCompression();
    bool validYN(string msg);
    bool initInput(int argc, char const *argv[]);    
    void renderQuadTreeAtDepth(QuadTreeNode* node, vector<uint8_t>& frameData, int imageWidth, int imageHeight, int maxDepth);
    int calculateNodeDepth(QuadTreeNode* root, QuadTreeNode* targetNode);
    bool createCompressionGif(const vector<vector<RGB>>& originalImage, const QuadTree& quadtree, int delayMs);

};

#endif