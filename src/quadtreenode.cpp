#include "header/quadtree.hpp"

/*
PART 1: QuadTreeNode
*/
QuadTreeNode::QuadTreeNode(): width(-1), height(-1), x(-1), y(-1){}

QuadTreeNode::QuadTreeNode(int x, int y, int width, int height): width(width), height(height), x(x), y(y){}

QuadTreeNode::~QuadTreeNode(){
  for (int i = 0; i < 4; i++) delete childNode[i];
  delete[] childNode;
}

int QuadTreeNode::getX() const {
  return x;
}

int QuadTreeNode::getY() const {
  return y;
}

pair<int,int> QuadTreeNode::getXY() const {
  return {x, y};
}

int QuadTreeNode::getWidth() const {
  return width;
}

int QuadTreeNode::getHeight() const {
  return height;
}

pair<int, int> QuadTreeNode::getSize() const {
  return {width, height};
}

bool QuadTreeNode::isLeafNode() const {
  return isLeaf;
}

RGB QuadTreeNode::getMean() const {
  return mean;
}

QuadTreeNode* QuadTreeNode::getChildNode(int idx) const{
  if (idx >= 4 || idx < 0) return nullptr;
  return childNode[idx];
}

void QuadTreeNode::setMean(RGB mean) {
  this->mean = mean;
}

void QuadTreeNode::setLeaf(bool isLeaf){
  this->isLeaf = isLeaf;
}

void QuadTreeNode::setChild(int idx, QuadTreeNode* node){
  this->childNode[idx] = node;
}