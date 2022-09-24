#ifndef CONVERTER_H
#define CONVERTER_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct RGBPixel
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct RGBAPixel
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

struct HeaderInfo
{
    char ppmVersion;
    int width;
    int height;
    int maxColor;
    int depth;
};

int getNextInt(FILE *fh);
int getNextPixel(FILE *fh);
void skipComments(FILE *fh);
int verifyPPMVersion(int ppm_version);
int verifyTupleType(FILE *fh, int depth);
int colorInBounds(int maxValue, int currColorVal);
int validateOutputPath(FILE *fh, char *outputFilename);
int readHeaderP7(FILE *fh, struct HeaderInfo *headerData);
int readHeaderP3P6(FILE *fh, struct HeaderInfo *headerData);
int readPPM3(FILE *fh, struct RGBPixel *rgbArray, struct HeaderInfo *headerData);
int readPPM6(FILE *fh, struct RGBPixel *rgbArray, struct HeaderInfo *headerData);
int readPPM7(FILE *fh, struct RGBAPixel *rgbaArray, struct HeaderInfo *headerData);
int writePPM3(struct RGBPixel *rgbArray, struct HeaderInfo *headerData, char *outputFilename);
int writePPM6(struct RGBPixel *rgbArray, struct HeaderInfo *headerData, char *outputFilename);
int writePPM7RGB(struct RGBPixel *rgbArray, struct HeaderInfo *headerData, char *outputFilename);
int writePPM7RGBAlpha(struct RGBAPixel *rgbaArray, struct HeaderInfo *headerData, char *outputFilename);
int convertRGBAToRGB(struct RGBAPixel *rgbaArray, struct RGBPixel *rgbArray, struct HeaderInfo *headerData);
#endif