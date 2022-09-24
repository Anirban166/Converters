#include "converter.h"

int readPPM6(FILE *fh, struct RGBPixel *rgbArray, struct HeaderInfo *headerData)
{
    int totalPixels = headerData->height * headerData->width;
    uint8_t *temp_data = (uint8_t *)malloc(totalPixels * 3 * sizeof(uint8_t));
    int scancount = fread(temp_data, sizeof(struct RGBPixel), totalPixels, fh);

    if(scancount != totalPixels)
    {
        fprintf(stderr, "Not enough data in the body of the inputted file.\n");
        return -1;
    }

    fgetc(fh);
    if (!feof(fh))
    {
        fprintf(stderr, "Additional data in the body of the inputted file (more characters exist after the expected body length!).\n");
        return -1;
    }

    for(int i = 0; i < totalPixels; i++)
    {
        int data_pointer = 3 * i;
        rgbArray[i].red = temp_data[data_pointer];
        data_pointer++;
        rgbArray[i].green = temp_data[data_pointer];
        data_pointer++;
        rgbArray[i].blue = temp_data[data_pointer];
    }
    printf("Finished reading P6 file.\n");
    free(temp_data);

    return 0;
}

int writePPM6(struct RGBPixel *rgbArray, struct HeaderInfo *headerData, char *outputFilename)
{
    FILE *fhWrite = fopen(outputFilename, "wb");
    if(validateOutputPath(fhWrite, outputFilename) == -1)
    {
        return -1;
    }
    fwrite("P6\n", 1, sizeof(char) * 3, fhWrite);
    fprintf(fhWrite, "%d %d\n", headerData->width, headerData->height);
    fprintf(fhWrite, "%d\n", headerData->maxColor);

    int size = headerData->width * headerData->height;
    for(int i = 0; i < size; i++)
    {
        struct RGBPixel currPixel = rgbArray[i];
        fwrite((uint8_t *)&(currPixel.red), sizeof(uint8_t), 1, fhWrite);
        fwrite((uint8_t *)&(currPixel.green), sizeof(uint8_t), 1, fhWrite);
        fwrite((uint8_t *)&(currPixel.blue), sizeof(uint8_t), 1, fhWrite);
    }
    printf("Finished writing P6 file.\n");
    fclose(fhWrite);
    return 0;
}
