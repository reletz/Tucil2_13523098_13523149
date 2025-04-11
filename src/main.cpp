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

    if (!stream.initInput(argc, argv)) return 0;

    auto start = chrono::steady_clock::now();

    vector<vector<RGB>> originalImage;
    if (stream.GENERATE_GIF) {
        originalImage = imageToMatrix(stream.imageSrcPath.string());
    }

    QuadTree qt = compressImage(stream);

    if (stream.GENERATE_GIF) {
        if (!qt.createCompressionGif(originalImage, stream.gifPath.string(), 200)) {
            cerr << "GIF creation failed" << endl;
        }
    }

    auto end = chrono::steady_clock::now();
    auto diff = end - start;

    cout << chrono::duration<double, milli>(end - start).count() << " ms" << '\n';
    cout << "Max tree depth: " << qt.getMaxDepth() << '\n';
    cout << "Total nodes: " << qt.getNodeCount() << '\n';
    return 0;
}