#include <mutex>
#include "opencv2/opencv.hpp"
#include "header/image_utils.hpp"
using namespace std;


vector<vector<RGB>> ImageUtils::imageToMatrix(const string& filename){
    cv::Mat img = cv::imread(filename);
    if (img.empty()){
        cout << "Could not read the image: " << filename << endl;
        exit(1);
    }

    int rows = img.rows;
    int cols = img.cols;
    vector<vector<RGB>> image(rows, vector<RGB>(cols));

    for(int i = 0; i < rows;i++){
        for(int j = 0 ; j < cols;j++){
            cv::Vec3b pixel = img.at<cv::Vec3b>(i,j);
            image[i][j].r = pixel[2];
            image[i][j].g = pixel[1];
            image[i][j].b = pixel[0];
        }
    }

    return image;
}

void ImageUtils::matrixToImage(const vector<vector<RGB>>& image, const string& filename){
    int rows = image.size();
    int cols = image[0].size();
    cv::Mat img(rows, cols, CV_8UC3);

    for(int i = 0; i < rows;i++){
        for(int j = 0 ; j < cols;j++){
            cv::Vec3b pixel;
            pixel[2] = image[i][j].r;
            pixel[1] = image[i][j].g;
            pixel[0] = image[i][j].b;
            img.at<cv::Vec3b>(i,j) = pixel;
        }
    }

    cv::imwrite(filename, img);
}

void ImageUtils::printMatrix(const vector<vector<RGB>>& image){
    int rows = image.size();
    int cols = image[0].size();

    for(int i = 0; i < rows;i++){
        for(int j = 0 ; j < cols;j++){
            cout << "(" << image[i][j].r << "," << image[i][j].g << "," << image[i][j].b << ") ";
        }
        cout << endl;
    }
}

vector<vector<RGB>> ImageUtils::resize(const vector<vector<RGB>>& image, int newWidth, int newHeight){
    int rows = image.size();
    int cols = image[0].size();
    vector<vector<RGB>> newImage(newHeight, vector<RGB>(newWidth));

    for(int i = 0; i < newHeight;i++){
        for(int j = 0 ; j < newWidth;j++){
            int x = i * rows / newHeight;
            int y = j * cols / newWidth;
            newImage[i][j] = image[x][y];
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
            newImage[i][j].r = image[i][j].r - mean.r;
            newImage[i][j].g = image[i][j].g - mean.g;
            newImage[i][j].b = image[i][j].b - mean.b;
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
        
        for (int i = y; i < y + height; ++i) {
            for (int j = x; j < x + width; ++j) {
                image[i][j] = mean;
            }
        }
    } else {
        for (int k = 0; k < 4; ++k) {
            fillCompressedImage(node->getChildNode(k), image);
        }
    }
    
}