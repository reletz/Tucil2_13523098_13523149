#include "header/image_error.hpp"
#include "header/image_utils.hpp"
#include "header/quadtree.hpp"
#include "header/io.hpp"

#include <ctime>
#include <filesystem>

using namespace std;
using namespace ImageUtils;

int main(int argc, char const *argv[])
{
    IO stream;
    while (true) {
        if (!stream.initInput(argc, argv)) return 0;

        auto start = chrono::steady_clock::now();

        vector<vector<RGB>> originalImage;
        if (stream.GENERATE_GIF) {
            originalImage = imageToMatrix(stream.imageSrcPath.string());
        }
        uintmax_t originalSize = filesystem::file_size(stream.imageSrcPath);
        QuadTree qt = compressImage(stream);
        if (qt.getNodeCount() == 0) {
            cout << "Error: Try to Input Image Again." << endl;
            continue;
        }
        auto end = chrono::steady_clock::now();
        auto diff = end - start;

        if (stream.GENERATE_GIF) {
            if (!qt.createCompressionGif(originalImage, stream.gifPath.string(), 200)) {
                cerr << "GIF creation failed" << endl;
            }
        }
        uintmax_t compressedSize = 0;
        if (stream.imageDestPath.empty() == false) {
            compressedSize = filesystem::file_size(stream.imageDestPath);
        }
        double compressionPercentage = 100.0 - ((double)compressedSize / originalSize * 100.0);
        cout << "Execution time: " << chrono::duration<double, milli>(diff).count() << " ms" << '\n';
        cout << "Original image size: " << originalSize << " bytes" << '\n';
        cout << "Compressed image size: " << compressedSize << " bytes" << '\n';
        cout << "Compression percentage: " << compressionPercentage << "%" << '\n';
        cout << "Max tree depth: " << qt.getMaxDepth() << '\n';
        cout << "Total nodes: " << qt.getNodeCount() << '\n';

        if (!stream.validYN("Do you want to continue compressing another image? (Y/n)")) return 0;
    }
    return 0;
}