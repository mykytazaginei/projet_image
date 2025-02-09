#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "image.h"

#include <string.h>


void generateImage(Image *img) {
    srand(time(NULL));
    for (int y = 0; y < img->sizeY; y++) {
        for (int x = 0; x < img->sizeX; x++) {
            img->data[y][x].r = (unsigned char)(rand() % 256);
            img->data[y][x].v = (unsigned char)(rand() % 256);
            img->data[y][x].b = (unsigned char)(rand() % 256);
        }
    }
}

void saveImage(Image *img, const char *filename){
    FILE *fb = fopen(filename, "wb");
    if (fb == NULL) {
        return;
    }
    fprintf(fb, "P6\n%d %d\n255\n", img->sizeX, img->sizeY);

    for (int y = 0; y < img->sizeY; y++) {
        for (int x = 0; x < img->sizeX; x++) {
            Pixel p = img->data[y][x];
            fwrite(&p.r, sizeof(unsigned char), 1, fb);
            fwrite(&p.v, sizeof(unsigned char), 1, fb);
            fwrite(&p.b, sizeof(unsigned char), 1, fb);
        }
    }

    fclose(fb);
}

void loadImage(Image *img, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return;
    }

    char buffer[128];
    // First line
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        perror("Error reading the first line");
        fclose(fp);
        return;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    if (strncmp(buffer, "P6", 2) != 0) {
        fprintf(stderr, "Invalid file format: expected P6, got: %s\n", buffer);
        fclose(fp);
        return;
    }

    // Second line
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        fprintf(stderr, "Error reading the dimensions line.\n");
        fclose(fp);
        return;
    }

    if (sscanf(buffer, "%d %d", &img->sizeX, &img->sizeY) != 2) {
        fprintf(stderr, "Invalid format for the dimensions line.\n");
        fclose(fp);
        return;
    }

    // Third line
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        fprintf(stderr, "Error reading the max color value line.\n");
        fclose(fp);
        return;
    }

    int maxColor;
    if (sscanf(buffer, "%d", &maxColor) != 1) {
        fprintf(stderr, "Invalid format for the max color value line.\n");
        fclose(fp);
        return;
    }

    if (maxColor != 255) {
        fprintf(stderr, "Invalid max color value: %d (expected 255).\n", maxColor);
        fclose(fp);
        return;
    }

    img->data = malloc(img->sizeY * sizeof(Pixel *));
    if (img->data == NULL) {
        fclose(fp);
        return;
    }
    for (int y = 0; y < img->sizeY; y++) {
        img->data[y] = malloc(img->sizeX * sizeof(Pixel));
        if (img->data[y] == NULL) {
            perror("Error allocating memory for an image row");
            for (int j = 0; j < y; j++) {
                free(img->data[j]);
            }
            free(img->data);
            fclose(fp);
            return;
        }
    }

    for (int y = 0; y < img->sizeY; y++) {
        for (int x = 0; x < img->sizeX; x++) {
            if (fread(&img->data[y][x].r, sizeof(unsigned char), 1, fp) != 1 ||
                fread(&img->data[y][x].v, sizeof(unsigned char), 1, fp) != 1 ||
                fread(&img->data[y][x].b, sizeof(unsigned char), 1, fp) != 1) {
                fprintf(stderr, "Error reading pixel data at position (%d, %d)\n", x, y);
                for (int j = 0; j < img->sizeY; j++) {
                    free(img->data[j]);
                }
                free(img->data);
                fclose(fp);
                return;
            }
        }
    }
    fclose(fp);
}

void freeImage(Image *img) {
    for (int i = 0; i < img->sizeY; i++) {
        free(img->data[i]);
    }
    free(img->data);
}

void colorFilter(Image *img, const char color) {
    for (int y = 0; y < img->sizeY; y++) {
        for (int x = 0; x < img->sizeX; x++) {
            if (color == 'r') {
                img->data[y][x].v = 0;
                img->data[y][x].b = 0;
            }
            else if (color == 'v') {
                img->data[y][x].r = 0;
                img->data[y][x].b = 0;
            }
            else if (color == 'b') {
                img->data[y][x].v = 0;
                img->data[y][x].r = 0;
            }
        }
    }
}

void grayscaleAverage(Image *img) {
    for (int y = 0; y < img->sizeY; y++) {
        for (int x = 0; x < img->sizeX; x++) {
            unsigned char r = img->data[y][x].r;
            unsigned char v = img->data[y][x].v;
            unsigned char b = img->data[y][x].b;
            unsigned char avg = (r + v + b) / 3;
            img->data[y][x].r = avg;
            img->data[y][x].v = avg;
            img->data[y][x].b = avg;
        }
    }
}

void grayscaleMax(Image *img) {
    for (int y = 0; y < img->sizeY; y++) {
        for (int x = 0; x < img->sizeX; x++) {
            unsigned char r = img->data[y][x].r;
            unsigned char v = img->data[y][x].v;
            unsigned char b = img->data[y][x].b;
            unsigned char m = r;
            if (v > m) m = v;
            if (b > m) m = b;
            img->data[y][x].r = m;
            img->data[y][x].v = m;
            img->data[y][x].b = m;
        }
    }
}

void sepiaFilter(Image *img) {
    for (int y = 0; y < img->sizeY; y++) {
        for (int x = 0; x < img->sizeX; x++) {
            unsigned char origR = img->data[y][x].r;
            unsigned char origG = img->data[y][x].v;
            unsigned char origB = img->data[y][x].b;

            int sepiaR = (int)(0.393 * origR + 0.769 * origG + 0.189 * origB);
            int sepiaG = (int)(0.349 * origR + 0.686 * origG + 0.168 * origB);
            int sepiaB = (int)(0.272 * origR + 0.534 * origG + 0.131 * origB);

            // Ограничиваем значения до 255, если они превышают этот предел
            if (sepiaR > 255) sepiaR = 255;
            if (sepiaG > 255) sepiaG = 255;
            if (sepiaB > 255) sepiaB = 255;

            img->data[y][x].r = (unsigned char)sepiaR;
            img->data[y][x].v = (unsigned char)sepiaG;
            img->data[y][x].b = (unsigned char)sepiaB;
        }
    }
}

void smoothImage(Image *img) {
    Pixel **temp = malloc(img->sizeY * sizeof(Pixel *));
    if (temp == NULL) {
        fprintf(stderr, "Error allocating memory for temporary array\n");
        return;
    }
    for (int y = 0; y < img->sizeY; y++) {
        temp[y] = malloc(img->sizeX * sizeof(Pixel));
        if (temp[y] == NULL) {
            fprintf(stderr, "Error allocating memory for temporary row\n");
            for (int j = 0; j < y; j++) {
                free(temp[j]);
            }
            free(temp);
            return;
        }
    }

    // Apply the smoothing filter using a 3x3 window
    for (int y = 0; y < img->sizeY; y++) {
        for (int x = 0; x < img->sizeX; x++) {
            int sumR = 0, sumG = 0, sumB = 0;
            int count = 0;
            // Loop through the 3x3 window around the current pixel
            for (int j = y - 1; j <= y + 1; j++) {
                for (int i = x - 1; i <= x + 1; i++) {
                    // Check that the indices are within the image boundaries
                    if (i >= 0 && i < img->sizeX && j >= 0 && j < img->sizeY) {
                        sumR += img->data[j][i].r;
                        sumG += img->data[j][i].v;
                        sumB += img->data[j][i].b;
                        count++;
                    }
                }
            }
            // Compute the average for each color channel and store in the temporary array
            temp[y][x].r = (unsigned char)(sumR / count);
            temp[y][x].v = (unsigned char)(sumG / count);
            temp[y][x].b = (unsigned char)(sumB / count);
        }
    }

    for (int y = 0; y < img->sizeY; y++) {
        for (int x = 0; x < img->sizeX; x++) {
            img->data[y][x] = temp[y][x];
        }
    }

    // Free the temporary memory
    for (int y = 0; y < img->sizeY; y++) {
        free(temp[y]);
    }
    free(temp);
}


