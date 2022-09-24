#include "converter.h"

int readPPM7(FILE *fh, struct RGBAPixel *rgbaArray, struct HeaderInfo *headerData)
{
    int totalPixels = headerData->height * headerData->width;
    uint8_t *temp_data = (uint8_t *)malloc(totalPixels * 4 * sizeof(uint8_t));
    int scancount = fread(temp_data, sizeof(struct RGBAPixel), totalPixels, fh);

    if(scancount != totalPixels)
    {
        fprintf(stderr, "Not enough data in the body of the inputted file.\n");
        return -1;
    }

    fgetc(fh);
    if(!feof(fh))
    {
        fprintf(stderr, "Additional data in the body of the inputted file (more characters exist after the expected body length!).\n");
        return -1;
    }

    for(int i = 0; i < totalPixels; i++)
    {
        int data_pointer = 4 * i;
        rgbaArray[i].red = temp_data[data_pointer];
        data_pointer++;
        rgbaArray[i].green = temp_data[data_pointer];
        data_pointer++;
        rgbaArray[i].blue = temp_data[data_pointer];
        data_pointer++;
        rgbaArray[i].alpha = temp_data[data_pointer];
    }
    printf("Finished reading P7 file.\n");
    free(temp_data);
    return 0;
}

int writePPM7RGB(struct RGBPixel *rgbArray, struct HeaderInfo *headerData, char *outputFilename)
{
    FILE *fhWrite = fopen(outputFilename, "wb");
    if(validateOutputPath(fhWrite, outputFilename) == -1)
    {
        return -1;
    }

    fwrite("P7\n", 1, sizeof(char) * 3, fhWrite);
    fprintf(fhWrite, "WIDTH %d\n", headerData->width);
    fprintf(fhWrite, "HEIGHT %d\n", headerData->height);
    fprintf(fhWrite, "DEPTH %d\n", 4);
    fprintf(fhWrite, "MAXVAL %d\n", headerData->maxColor);
    fprintf(fhWrite, "TUPLTYPE RGB_ALPHA\n");
    fprintf(fhWrite, "ENDHDR\n");

    uint8_t defaultAlpha = headerData->maxColor;

    int size = headerData->width * headerData->height;
    for(int i = 0; i < size; i++)
    {
        struct RGBPixel currPixel = rgbArray[i];
        fwrite((uint8_t *)&(currPixel.red), sizeof(uint8_t), 1, fhWrite);
        fwrite((uint8_t *)&(currPixel.green), sizeof(uint8_t), 1, fhWrite);
        fwrite((uint8_t *)&(currPixel.blue), sizeof(uint8_t), 1, fhWrite);
        fwrite((uint8_t *)&defaultAlpha, sizeof(uint8_t), 1, fhWrite);
    }

    printf("Finished writing P7 file\n");
    fclose(fhWrite);
    return 0;
}

int writePPM7RGBAlpha(struct RGBAPixel *rgbaArray, struct HeaderInfo *headerData, char *outputFilename)
{
    FILE *fhWrite = fopen(outputFilename, "wb");
    if(validateOutputPath(fhWrite, outputFilename) == -1)
    {
        return -1;
    }

    fwrite("P7\n", 1, sizeof(char) * 3, fhWrite);
    fprintf(fhWrite, "WIDTH %d\n", headerData->width);
    fprintf(fhWrite, "HEIGHT %d\n", headerData->height);
    fprintf(fhWrite, "DEPTH %d\n", headerData->depth);
    fprintf(fhWrite, "MAXVAL %d\n", headerData->maxColor);
    fprintf(fhWrite, "TUPLTYPE RGB_ALPHA\n");
    fprintf(fhWrite, "ENDHDR\n");

    int size = headerData->width * headerData->height;
    for(int i = 0; i < size; i++)
    {
        struct RGBAPixel currPixel = rgbaArray[i];
        fwrite((uint8_t *)&(currPixel.red), sizeof(uint8_t), 1, fhWrite);
        fwrite((uint8_t *)&(currPixel.green), sizeof(uint8_t), 1, fhWrite);
        fwrite((uint8_t *)&(currPixel.blue), sizeof(uint8_t), 1, fhWrite);
        fwrite((uint8_t *)&(currPixel.alpha), sizeof(uint8_t), 1, fhWrite);
    }
    printf("Finished writing P7 file.\n");
    fclose(fhWrite);
    return 0;
}
