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

  // cout << this->method << endl;

  if(this->method == 4 ){
    if (error > threshold || width <= minSize || height <= minSize) {
      RGB mean = ImageError::mean(image, x, y, width, height);
      node -> setMean(mean);
      node -> setLeaf(true);
      return node;
    }
  }else{
    if (error < threshold || width <= minSize || height <= minSize) {
      RGB mean = ImageError::mean(image, x, y, width, height);
      node -> setMean(mean);
      node -> setLeaf(true);
      return node;
    }
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

void QuadTree::renderFrameToImage(QuadTreeNode* node, vector<uint8_t>& frameData, int imageWidth, int imageHeight, int currentDepth, int maxDepth) {
  if (node == nullptr) return;
  
  if (node->isLeafNode() || currentDepth <= 0) {
      RGB mean = node->getMean();
      int startX = node->getX();
      int startY = node->getY();
      int width = node->getWidth();
      int height = node->getHeight();
      
      for (int y = startY; y < startY + height && y < imageHeight; y++) {
          for (int x = startX; x < startX + width && x < imageWidth; x++) {
              int flippedY = imageHeight - 1 - y;
              int idx = (flippedY * imageWidth + x) * 3;
              
              if (idx >= 0 && idx + 2 < frameData.size()) {
                  frameData[idx] = mean.r;
                  frameData[idx + 1] = mean.g;
                  frameData[idx + 2] = mean.b;
              }
          }
      }
  } else if (currentDepth > 0) {
      for (int i = 0; i < 4; i++) {
          renderFrameToImage(node->getChildNode(i), frameData, imageWidth, imageHeight, currentDepth - 1, maxDepth);
      }
  }
}


void QuadTree::collectFramesRecursive(QuadTreeNode* node, vector<vector<uint8_t>>& frames, int imageWidth, int imageHeight, int& currentDepth) {
  int maxDepth = getMaxDepth();
  
  for (int depth = 1; depth <= maxDepth; depth++) {
      vector<uint8_t> frameData(imageWidth * imageHeight * 3, 0);
      
      renderFrameToImage(root, frameData, imageWidth, imageHeight, depth, maxDepth);
      
      frames.push_back(frameData);
  }
}

bool QuadTree::createCompressionGif(const vector<vector<RGB>>& originalImage, const string& gifPath, int delayMs) {
  if (root == nullptr) {
      cerr << "[ERROR] Cannot create GIF: QuadTree not built yet" << endl;
      return false;
  }
  
  int height = originalImage.size();
  if (height == 0) return false;
  int width = originalImage[0].size();
  if (width == 0) return false;
  
  vector<vector<uint8_t>> frames;
  
  int currentDepth = 0;
  collectFramesRecursive(root, frames, width, height, currentDepth);
  
  bool success = createAnimatedGif(frames, width, height, delayMs, gifPath);
  
  if (success) {
      cout << "[INFO] GIF saved successfully at: " << gifPath << endl;
  } else {
      cerr << "[ERROR] Failed to create GIF" << endl;
  }
  
  return success;
}
