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
  int method, double threshold, int minSize
) {
  this->method = method;
  this->threshold = threshold;
  this->minSize = minSize;

  this->imageOriginal = image;
  this->imageReconstructed = vector<vector<RGB>>(image.size(), vector<RGB>(image[0].size()));
  this->root = buildRecursive(x, y, width, height);
}

QuadTreeNode* QuadTree::buildRecursive(int x, int y, int width, int height) {
  QuadTreeNode* node = new QuadTreeNode(x, y, width, height);

  float error = calculateError(imageOriginal, imageReconstructed, x, y, width, height, method);

  if (error < threshold || width <= minSize || height <= minSize) {
    RGB mean = ImageError::mean(imageOriginal, x, y, width, height);
    node->setMean(mean);
    node->setLeaf(true);

    for (int j = y; j < y + height; ++j) {
      for (int i = x; i < x + width; ++i) {
        imageReconstructed[j][i] = mean;
      }
    }

    return node;
  }

  node->setLeaf(false);
  int halfW = width / 2;
  int halfH = height / 2;

  node->setChild(0, buildRecursive(x, y, halfW, halfH));
  node->setChild(1, buildRecursive(x + halfW, y, width - halfW, halfH));
  node->setChild(2, buildRecursive(x, y + halfH, halfW, height - halfH));
  node->setChild(3, buildRecursive(x + halfW, y + halfH, width - halfW, height - halfH));

  return node;
}

// formulation
float QuadTree::calculateError(const vector<vector<RGB>>& imageA, 
  const vector<vector<RGB>>& imageB, 
  int x, int y, int width, int height, int method) {
  switch(method){
    case 0: return ImageError::variance(imageA, x, y, width, height);
    case 1: return ImageError::mad(imageA, x, y, width, height);
    case 2: return ImageError::maxDiff(imageA, x, y, width, height);
    case 3: return ImageError::entropy(imageA, x, y, width, height);
    case 4: return 1.0f -  ImageError::ssim(imageA, imageB, x, x, y, y, width, height);
    default: 
      cerr << "[ERROR] Unknown error method: " << method << '\n';
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