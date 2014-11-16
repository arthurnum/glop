#include <stdio.h>


typedef struct bmp_image
{
    unsigned int width, height;
    unsigned char *data;
} bmp_image;


bmp_image load_bmp_image(char *filename)
{
    bmp_image image;
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int imageSize;   // = width*height*3

    FILE *file = fopen(filename,"rb");
    fread(header, 1, 54, file);
    // Read ints from the byte array
    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    image.width = *(int*)&(header[0x12]);
    image.height = *(int*)&(header[0x16]);
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0) imageSize = image.width * image.height * 3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0) dataPos = 54; // The BMP header is done that way
    // Create a buffer
    image.data = malloc(imageSize*sizeof(unsigned char));

    // Read the actual data from the file into the buffer
    fread(image.data, 1, imageSize, file);
    for (int i = 0; i < imageSize; i += 3)
    {
        unsigned char buf = image.data[i];
        image.data[i] = image.data[i+2];
        image.data[i+2] = buf;
    }

    //Everything is in memory now, the file can be closed
    fclose(file);
    return image;
}
