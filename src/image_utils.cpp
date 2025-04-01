#include <mutex>
#include "header/image_utils.hpp"
using namespace std;

vector<vector<RGB>> ImageUtils::imageToMatrix(const string& filename){
    fipImage img;
    img.load(filename.c_str());
    if (!img.isValid()) {
        cout << "Could not read the image: " << filename << endl;
        exit(1);
    }    

    int x = img.getWidth();
    int y = img.getHeight();
    vector<vector<RGB>> image(y, vector<RGB>(x));

    for (int i = 0; i < y; i++){
        for (int j = 0 ; j < x; j++){
            RGBQUAD color;
            if (img.getPixelColor(j, i, &color)) {  // Perbaikan indeks
                image[i][j].r = color.rgbRed;
                image[i][j].g = color.rgbGreen;
                image[i][j].b = color.rgbBlue;
            }
        }
    }

    return image;
}

void ImageUtils::matrixToImage(const vector<vector<RGB>>& image, const string& filename) {
    if (image.empty() || image[0].empty()) {  // Cek agar tidak segmentation fault
        cout << "Error: Image matrix is empty!" << endl;
        return;
    }

    int height = image.size();
    int width = image[0].size();

    fipImage img(FIT_BITMAP, width, height, 24);
    if (!img.isValid()) {
        cout << "Failed to create image!" << endl;
        return;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            RGBQUAD color;
            color.rgbRed = image[y][x].r;
            color.rgbGreen = image[y][x].g;
            color.rgbBlue = image[y][x].b;
            img.setPixelColor(x, y, &color);
        }
    }

    if (!img.save(filename.c_str())) {
        cout << "Failed to save image: " << filename << endl;
    }
}

vector<vector<RGB>> ImageUtils::resize(const vector<vector<RGB>>& image, int newWidth, int newHeight){
    int rows = image.size();
    int cols = image[0].size();
    vector<vector<RGB>> newImage(newHeight, vector<RGB>(newWidth));

    for(int i = 0; i < newHeight;i++){
        for(int j = 0 ; j < newWidth;j++){
            int y = i * rows / newHeight;  // Perbaikan indeks
            int x = j * cols / newWidth;  
            newImage[i][j] = image[y][x];
        }
    }

    return newImage;
}

vector<vector<RGB>> ImageUtils::normalize(const vector<vector<RGB>>& image, RGB mean){
    int rows = image.size();
    int cols = image[0].size();
    vector<vector<RGB>> newImage(rows, vector<RGB>(cols));

    for(int i = 0; i < rows;i++){
        for(int j = 0 ; j < cols;j++){
            newImage[i][j].r = max(0, image[i][j].r - mean.r);
            newImage[i][j].g = max(0, image[i][j].g - mean.g);
            newImage[i][j].b = max(0, image[i][j].b - mean.b);
        }
    }

    return newImage;
}

void ImageUtils::fillCompressedImage(QuadTreeNode* node, vector<vector<RGB>>& image){
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

QuadTree ImageUtils::compressImage(IO streams){
    vector<vector<RGB>> image = imageToMatrix(streams.imageSrcPath.string());
    QuadTree qt;
    qt.buildTree(image, 0, 0, image[0].size(), image.size(), streams.method-1, streams.VAR_THRESHOLD, streams.MIN_BLOCK_SIZE);
    vector<vector<RGB>> compressedImage(image.size(), vector<RGB>(image[0].size()));
    fillCompressedImage(qt.getRoot(), compressedImage);
    matrixToImage(compressedImage, streams.imageDestPath.string());

    return qt;
}
