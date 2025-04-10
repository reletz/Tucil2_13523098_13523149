#include "header/quadtree.hpp"
#include "header/image_error.hpp"

/*
PART 2: QuadTree
*/

// ctor-dtor
QuadTree::QuadTree(): method(-1), threshold(-1), minSize(-1), root(nullptr) {}
QuadTree::~QuadTree() {
  if (root != nullptr) {
    delete root;
    root = nullptr;
  }
}


// getter
QuadTreeNode* QuadTree::getRoot() const { return root; }
int QuadTree::getMethod() const { return method; }
int QuadTree::getMinSize() const { return minSize; }

// tree-building
void QuadTree::buildTree(const vector<vector<RGB>>& image, 
  int x, int y, 
  int width, int height, 
  int method, float threshold, int minSize
) {
  this->method = method;
  this->threshold = threshold;
  this->minSize = minSize;

  this->root = buildRecursive(image, x, y, width, height);
}

QuadTreeNode* QuadTree::buildRecursive(const vector<vector<RGB>>& image, int x, int y, int width, int height){
  QuadTreeNode* node = new QuadTreeNode(x, y, width, height);

  float error = calculateError(image, x, y, width, height, method);
  // cout << "[INFO] Error: " << error << " | Threshold: " << threshold << '\n';

  // Kalau udah cukup (udah jadi leaf)
  if (error > threshold || width <= minSize || height <= minSize) {
    RGB mean = ImageError::mean(image, x, y, width, height);
    node -> setMean(mean);
    node -> setLeaf(true);
    return node;
  }

  node -> setLeaf(false);
  int halfW = width / 2;
  int halfH = height / 2;

  // bikin anak
  node->setChild(0, buildRecursive(image, x, y, halfW, halfH));
  node->setChild(1, buildRecursive(image, x + halfW, y, width - halfW, halfH));
  node->setChild(2, buildRecursive(image, x, y + halfH, halfW, height - halfH));
  node->setChild(3, buildRecursive(image, x + halfW, y + halfH, width - halfW, height - halfH));

  return node;
}

// formulation
float QuadTree::calculateError(const std::vector<std::vector<RGB>>& image, int x, int y, int width, int height, int method) {
  switch(method){
    case 0: return ImageError::variance(image, x, y, width, height);
    case 1: return ImageError::mad(image, x, y, width, height);
    case 2: return ImageError::maxDiff(image, x, y, width, height);
    case 3: return ImageError::entropy(image, x, y, width, height);
    case 4: return ImageError::ssim(image, x, y, width, height);
    default: 
      // cerr << "[ERROR] Unknown error method: " << method << '\n';
      return 0.0f;
  }
}

int QuadTree::getMaxDepth(QuadTreeNode* node) const {
  if (node == nullptr) return 0;
  
  if (node->isLeafNode()) return 1;

  int maxChildDepth = 0;
  for (int i = 0; i < 4; i++) {
      maxChildDepth = max(maxChildDepth, getMaxDepth(node->getChildNode(i)));
  }

  return 1 + maxChildDepth;
}

int QuadTree::getMaxDepth() const {
  return getMaxDepth(root);
}

int QuadTree::getNodeCount(QuadTreeNode* node) const {
  if (node == nullptr) return 0;

  int count = 1; // Hitung diri sendiri

  for (int i = 0; i < 4; i++) {
      count += getNodeCount(node->getChildNode(i));
  }

  return count;
}

int QuadTree::getNodeCount() const {
  return getNodeCount(root);
}