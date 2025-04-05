#include <mutex>
#include "header/image_utils.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

vector<vector<RGB>> ImageUtils::imageToMatrix(const string& filename) {
    int width, height, channels;
    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 3); // Force 3 channels (RGB)
    
    if (!img) {
        cout << "Could not read the image: " << filename << endl;
        exit(1);
    }
    
    vector<vector<RGB>> image(height, vector<RGB>(width));
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int idx = (i * width + j) * 3; // Calculate position in the array
            image[i][j].r = img[idx];      // Red
            image[i][j].g = img[idx + 1];  // Green
            image[i][j].b = img[idx + 2];  // Blue
        }
    }
    
    stbi_image_free(img);
    return image;
}

void ImageUtils::matrixToImage(const vector<vector<RGB>>& image, const string& filename) {
    if (image.empty() || image[0].empty()) {
        cout << "Error: Image matrix is empty!" << endl;
        return;
    }

    int height = image.size();
    int width = image[0].size();
    
    unsigned char* img_data = new unsigned char[width * height * 3];
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int idx = (i * width + j) * 3;
            img_data[idx] = image[i][j].r;     // Red
            img_data[idx + 1] = image[i][j].g; // Green
            img_data[idx + 2] = image[i][j].b; // Blue
        }
    }
    
    int success = 0;
    
    string ext = filename.substr(filename.find_last_of(".") + 1);
    if (ext == "png") {
        success = stbi_write_png(filename.c_str(), width, height, 3, img_data, width * 3);
    } else if (ext == "jpg" || ext == "jpeg") {
        success = stbi_write_jpg(filename.c_str(), width, height, 3, img_data, 50);
    } else if (ext == "bmp") {
        success = stbi_write_bmp(filename.c_str(), width, height, 3, img_data);
    } else {
        success = stbi_write_png(filename.c_str(), width, height, 3, img_data, width * 3);
    }
    
    delete[] img_data;
    
    if (!success) {
        cout << "Failed to save image: " << filename << endl;
    }
}

vector<vector<RGB>> ImageUtils::resize(const vector<vector<RGB>>& image, int newWidth, int newHeight) {
    int rows = image.size();
    int cols = image[0].size();
    vector<vector<RGB>> newImage(newHeight, vector<RGB>(newWidth));

    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            int y = i * rows / newHeight;
            int x = j * cols / newWidth;  
            newImage[i][j] = image[y][x];
        }
    }

    return newImage;
}

vector<vector<RGB>> ImageUtils::normalize(const vector<vector<RGB>>& image, RGB mean) {
    int rows = image.size();
    int cols = image[0].size();
    vector<vector<RGB>> newImage(rows, vector<RGB>(cols));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            newImage[i][j].r = max(0, (int)image[i][j].r - (int)mean.r);
            newImage[i][j].g = max(0, (int)image[i][j].g - (int)mean.g);
            newImage[i][j].b = max(0, (int)image[i][j].b - (int)mean.b);
        }
    }

    return newImage;
}

void ImageUtils::fillCompressedImage(QuadTreeNode* node, vector<vector<RGB>>& image) {
    if (node->isLeafNode()) {
        int x = node->getX();
        int y = node->getY();
        int width = node->getWidth();
        int height = node->getHeight();
        RGB mean = node->getMean();
        
        for (int i = y; i < min(y + height, (int)image.size()); ++i) {
            for (int j = x; j < min(x + width, (int)image[0].size()); ++j) {
                image[i][j] = mean;
            }
        }
    } else {
        for (int k = 0; k < 4; ++k) {
            fillCompressedImage(node->getChildNode(k), image);
        }
    }
}

QuadTree ImageUtils::compressImage(IO streams) {
    vector<vector<RGB>> image = imageToMatrix(streams.imageSrcPath.string());
    QuadTree qt;
    qt.buildTree(image, 0, 0, image[0].size(), image.size(), streams.method-1, streams.VAR_THRESHOLD, streams.MIN_BLOCK_SIZE);
    vector<vector<RGB>> compressedImage(image.size(), vector<RGB>(image[0].size()));
    fillCompressedImage(qt.getRoot(), compressedImage);
    matrixToImage(compressedImage, streams.imageDestPath.string());

    return qt;
}