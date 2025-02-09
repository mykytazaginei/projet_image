//
// Created by Mykyta Zaginei on 09/02/2025.
//

#ifndef IMAGE_H
#define IMAGE_H


typedef struct {
    unsigned char r;
    unsigned char v;
    unsigned char b;
} Pixel;

typedef struct {
    int sizeX;
    int sizeY;
    Pixel **data;
} Image;

void generateImage(Image *img);

void saveImage(Image *img, const char *filename);

void loadImage(Image * img, const char *filename);

void freeImage(Image *img);

void colorFilter(Image *img, const char color);

void grayscaleAverage(Image *img);

void grayscaleMax(Image *img);

void sepiaFilter(Image *img);

void smoothImage(Image *img);
#endif //IMAGE_H
