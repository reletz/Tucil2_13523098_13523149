#ifndef _QUADTREE_H
#define _QUADTREE_H

#include <iostream>
#include <vector>

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
    int threshold;
    int minSize;
  
  public:
    QuadTree();
    ~QuadTree();

    QuadTreeNode* getRoot() const;
    int getMethod() const;
    int getThreshold() const;
    int getMinSize() const;

    float calculateError(const vector<vector<RGB>>& image, int x, int y, int width, int height, int method);

    void buildTree(const vector<vector<RGB>>& image, int x, int y, int width, int height, int method, int threshold, int minSize);
    QuadTreeNode* buildRecursive(const vector<vector<RGB>>& image, int x, int y, int width, int height);
};

#endif