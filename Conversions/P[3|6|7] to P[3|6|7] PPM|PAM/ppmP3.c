#include "converter.h"

int readPPM3(FILE *fh, struct RGBPixel *rgbArray, struct HeaderInfo *headerData)
{
    int index = 0, maxColor = headerData->maxColor;
    int totalPixels = headerData->width * headerData->height;

    if(feof(fh))
    {
        printf("The image Body is Empty.");
        return 1;
    }

    while(!feof(fh))
    {
        if(index > totalPixels)
        {
            fprintf(stderr, "Pixels in body does not match the dimensions given by width and height! Too many pixels given.\n");
            return -1;
        }

        int red = getNextPixel(fh);
        if(red == -1) break;
        rgbArray[index].red = red;
        if(!feof(fh)) rgbArray[index].green = getNextPixel(fh);
        if(!feof(fh)) rgbArray[index].blue = getNextPixel(fh);
        else
        {
            printf("Missing a green or blue value for the current pixel.\n");
            return 1;
        }

        if(!(colorInBounds(maxColor, rgbArray[index].red) && colorInBounds(maxColor, rgbArray[index].green) && colorInBounds(maxColor, rgbArray[index].blue)))
        {
            printf("One of the pixel colors is outside the valid color bounds.\n");
            return -1;
        }
        index++;
    }

    if(index != totalPixels)
    {
        fprintf(stderr, "Pixels in body do not match the dimensions given by width and height! (too few provided)\n");
        return -1;
    }
    printf("Finished reading P3 file.\n");
    return 0;
}

int writePPM3(struct RGBPixel *rgbArray, struct HeaderInfo *headerData, char *outputFilename)
{
    FILE *fhWrite = fopen(outputFilename, "w");
    if(validateOutputPath(fhWrite, outputFilename) == -1)
    {
        return -1;
    }
    fwrite("P3\n", 1, sizeof(char) * 3, fhWrite);
    fprintf(fhWrite, "%d %d\n", headerData->width, headerData->height);
    fprintf(fhWrite, "%d\n", headerData->maxColor);
    int size = headerData->width * headerData->height;

    for(int i = 0; i < size; i++)
    {
        struct RGBPixel currPixel = rgbArray[i];
        fprintf(fhWrite, "%d %d %d", currPixel.red, currPixel.green, currPixel.blue);
        if((i + 1) % 3 == 0)
        {
            fprintf(fhWrite, "\n");
        }
        else
        {
            fprintf(fhWrite, " ");
        }
    }
    printf("Finished writing P3 file.\n");
    fclose(fhWrite);
    return 0;
}
