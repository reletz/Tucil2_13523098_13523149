#include <mutex>
#include "header/image_utils.hpp"
using namespace std;

vector<vector<RGB>> ImageUtils::imageToMatrix(const string& filename){
    fipImage img;
    img.load(filename.c_str());
    // if(!img.isValid()) {
    //     cout << "Failed to load image: " << filename << endl;
    //     cout << "Try to use another image." << endl;
    //     return vector<vector<RGB>>();  
    // }

    int x = img.getWidth();
    int y = img.getHeight();

    if(x <= 0 || y <= 0) {
        cout << "Invalid image dimensions: " << x << "x" << y << endl;
        return vector<vector<RGB>>();  
    }
    
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
    // if (image.empty() || image[0].empty()) {
    //     cout << "Error: Image is not valid!" << endl;
    //     return QuadTree();
    // }
    QuadTree qt;
    qt.buildTree(image, 0, 0, image[0].size(), image.size(), streams.method-1, streams.VAR_THRESHOLD, streams.MIN_BLOCK_SIZE);
    vector<vector<RGB>> compressedImage(image.size(), vector<RGB>(image[0].size()));
    fillCompressedImage(qt.getRoot(), compressedImage);
    matrixToImage(compressedImage, streams.imageDestPath.string());

    return qt;
}
