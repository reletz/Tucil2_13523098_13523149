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

    stream.initInput(argc, argv);

    auto start = chrono::steady_clock::now();

    QuadTree qt = compressImage(stream);
    
    auto end = chrono::steady_clock::now();
    auto diff = end - start;

    cout << chrono::duration<double, milli>(end - start).count() << " ms" << '\n';
    cout << qt.getMaxDepth() << '\n';
    cout << qt.getNodeCount() << '\n';
    return 0;
}