#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

class image {
public:
    image () {};

    image(const char* filename) {
        std::string f = std::string(filename);

        auto imagedir = getenv("PROJECT_ROOT");
        if (imagedir && load(std::string(imagedir) + "/" + filename)) return;
        if (load(filename)) return;

        std::cerr << "Could not load image " << f << std::endl;
    }

    ~image() {
        STBI_FREE(fdata);
    }

    bool load(const std::string& filename) {
        auto n = bytesPerPixel;
        fdata = stbi_loadf(filename.c_str(),&imageWidth,&imageHeight,&n,bytesPerPixel);

        if (fdata == nullptr) return false;
        bytesPerScanline = imageWidth * bytesPerPixel;
        return true;
    }
    int width()  const { return (fdata == nullptr) ? 0 : imageWidth; }
    int height() const { return (fdata == nullptr) ? 0 : imageHeight; }

    const float* pixelData(int x, int y) const {
        static float magenta[] = {1,0,1};
        if (fdata == nullptr) return magenta;

        x = std::clamp(x,0,imageWidth-1);
        y = std::clamp(y,0,imageHeight-1);

        return fdata + y * bytesPerScanline + x * bytesPerPixel;

    }
private:
    const int bytesPerPixel = 3;
    float *fdata = nullptr;
    int imageWidth = 0;
    int imageHeight = 0;
    int bytesPerScanline=0;
};