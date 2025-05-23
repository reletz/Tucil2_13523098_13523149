#ifndef _QUADTREE_H
#define _QUADTREE_H

#include <iostream>
#include <vector>
#include "gif_maker.hpp"

using namespace std;

struct RGB{
  int r, g, b;
};

class QuadTreeNode{
  private:
    int width, height, x, y;
    bool isLeaf;
    RGB mean;
    QuadTreeNode* childNode[4];

  public:
    QuadTreeNode();
    QuadTreeNode(int x, int y, int width, int height);
    ~QuadTreeNode();

    int getX() const;
    int getY() const;
    pair<int,int> getXY() const;
    int getWidth() const;
    int getHeight() const;
    pair<int, int> getSize() const;

    bool isLeafNode() const;
    RGB getMean() const;
    QuadTreeNode* getChildNode(int idx) const;

    void setMean(RGB mean);
    void setLeaf(bool isLeaf);
    void setChild(int idx, QuadTreeNode* node);
};


class QuadTree{
  private:
    QuadTreeNode* root; // root
    int method;         // [0..4]
    float threshold;
    int minSize;
  
  public:
    QuadTree();
    ~QuadTree();

    QuadTreeNode* getRoot() const;
    int getMethod() const;
    float getThreshold() const;
    int getMinSize() const;

    int getMaxDepth() const;
    int getMaxDepth(QuadTreeNode* node) const;

    int getNodeCount() const;
    int getNodeCount(QuadTreeNode* node) const;

    float calculateError(const vector<vector<RGB>>& image, int x, int y, int width, int height, int method);

    void buildTree(const vector<vector<RGB>>& image, int x, int y, int width, int height, int method, float threshold, int minSize);
    QuadTreeNode* buildRecursive(const vector<vector<RGB>>& image, int x, int y, int width, int height);
    void renderFrameToImage(QuadTreeNode* node, vector<uint8_t>& frameData, int imageWidth, int imageHeight, int currentDepth, int maxDepth);
    void collectFramesRecursive(QuadTreeNode* node, vector<vector<uint8_t>>& frames, int imageWidth, int imageHeight, int& currentDepth);
    bool createCompressionGif(const vector<vector<RGB>>& originalImage, const string& gifPath, int delayMs = 100);
};

#endif