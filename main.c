#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "image.h"



int main(void) {
    // Generating a random image and saving it
    /*Image img;

    img.sizeX = 640;
    img.sizeY = 480;

    img.data = malloc(img.sizeY * sizeof(Pixel *));
    if (img.data == NULL) {
        fprintf(stderr, "Error allocating memory for the image\n");
        return -1;
    }
    for (int y = 0; y < img.sizeY; y++) {
        img.data[y] = malloc(img.sizeX * sizeof(Pixel));
        if (img.data[y] == NULL) {
            fprintf(stderr, "Error allocating memory for an image row\n");
            // Free the already allocated memory before exiting
            for (int i = 0; i < y; i++) {
                free(img.data[i]);
            }
            free(img.data);
            return -1;
        }
    }

    // Generate a random image
    generateImage(&img);

    // Save the generated image to the file "random.ppm"
    saveImage(&img, "random.ppm");

    // Free the memory allocated for the image
    freeImage(&img);*/

    // Loading an image from a file
    /*Image loadImg;
    loadImage(&loadImg, "random.ppm");
    saveImage(&loadImg, "loaded.ppm");
    freeImage(&loadImg);*/

    //Filter
    /*Image loadImg;
    loadImage(&loadImg, "random.ppm");
    colorFilter(&loadImg, 'b');
    saveImage(&loadImg, "filtredImg.ppm");
    freeImage(&loadImg);*/

    //Gray scale Average
    /*Image loadImg;
    loadImage(&loadImg, "random.ppm");
    grayscaleAverage(&loadImg);
    saveImage(&loadImg, "grayscaleAverage.ppm");
    freeImage(&loadImg);*/

    //Gray scale Max
    /*Image loadImg;
    loadImage(&loadImg, "random.ppm");
    grayscaleMax(&loadImg);
    saveImage(&loadImg, "grayscaleMax.ppm");
    freeImage(&loadImg);*/

    //Sepia Filter
    /*Image loadImg;
    loadImage(&loadImg, "random.ppm");
    sepiaFilter(&loadImg);
    saveImage(&loadImg, "sepiaFilter.ppm");
    freeImage(&loadImg);*/

    //smooth Image
    /*Image loadImg;
    loadImage(&loadImg, "random.ppm");
    smoothImage(&loadImg);
    saveImage(&loadImg, "smoothImage.ppm");
    freeImage(&loadImg);*/
    return 0;
}