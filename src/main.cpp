#include "header/image_error.hpp"
#include "header/image_utils.hpp"
#include "header/quadtree.hpp"
#include <filesystem>
using namespace std;

int main(int argc, char const *argv[])
{
    long long VAR_THRESHOLD = 0;
    long long MIN_BLOCK_SIZE = 0;
    std::filesystem::path rootWorkspace = std::filesystem::canonical(std::filesystem::path(argv[0]).parent_path() / "..");
    rootWorkspace += "/test/";
    
    cout << "Input Image Name in test folder: ";
    string imageName;
    cin >> imageName;
    cout << "Input Variance Threshold: ";
    cin >> VAR_THRESHOLD;
    cout << "Input Minimum Block Size: ";
    cin >> MIN_BLOCK_SIZE;

    int method = -1;
    cout << "1. Variance" << endl;
    cout << "2. Mean Absolute Deviation" << endl;
    cout << "3. Maximum Difference" << endl;
    cout << "4. Entropy" << endl;
    cout << "Choose method: " << endl;
    cin >> method;


    vector<vector<RGB>> image = ImageUtils::imageToMatrix((rootWorkspace / imageName).string());
    QuadTree qt;
    qt.buildTree(image, 0, 0, image[0].size(), image.size(), method-1, VAR_THRESHOLD, MIN_BLOCK_SIZE);
    vector<vector<RGB>> compressedImage(image.size(), vector<RGB>(image[0].size()));
    ImageUtils::fillCompressedImage(qt.getRoot(), compressedImage);
    ImageUtils::matrixToImage(compressedImage, (rootWorkspace / ("compressed_" + imageName)).string());
    return 0;
}
