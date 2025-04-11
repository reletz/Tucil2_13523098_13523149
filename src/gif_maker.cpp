#include "header/gif_maker.hpp"
#include "header/gif.h"
#include <cstdio>

bool createAnimatedGif(const std::vector<std::vector<uint8_t>> &frames,
                       int width, int height,
                       int delayMs,
                       const std::string &outputPath)
{
    if (frames.empty() || width <= 0 || height <= 0)
        return false;

    GifWriter writer = {};
    if (!GifBegin(&writer, outputPath.c_str(), width, height, delayMs))
        return false;

    // Each frame is assumed to be 3 channels (RGB).
    // gif.h expects 4 channels per pixel (RGBA), so we convert.
    for (const auto &frame : frames)
    {
        std::vector<uint8_t> rgba(width * height * 4);
        for (int i = 0; i < width * height; i++)
        {
            rgba[i * 4 + 0] = frame[i * 3 + 2];
            rgba[i * 4 + 1] = frame[i * 3 + 1];
            rgba[i * 4 + 2] = frame[i * 3 + 0];
            rgba[i * 4 + 3] = 255;
        }
        if (!GifWriteFrame(&writer, rgba.data(), width, height, delayMs))
        {
            GifEnd(&writer);
            return false;
        }
    }
    GifEnd(&writer);
    return true;
}