#include "converter.h"

int readHeaderP3P6(FILE *fh, struct HeaderInfo *headerData)
{
    int width = -1;
    int height = -1;
    int maxColor = -1;
    width = getNextInt(fh);
    height = getNextInt(fh);

    headerData->width = width;
    headerData->height = height;

    if(width <= 0 || height <= 0)
    {
        printf("Invalid file format - width, or height.\n");
        return -1;
    }

    printf("Width: %d, Height: %d\n", width, height);

    maxColor = getNextInt(fh);
    printf("Color: %d\n", maxColor);
    headerData->maxColor = maxColor;
    if(maxColor > 255 || maxColor <= 0)
    {
        printf("Invalid file format - width, or height.\n");
        return -1;
    }
    // Setting depth to 3 channels since P3 and P6 only support RGB:
    headerData->depth = 3;

    while(fgetc(fh) != '\n');

    return 0;
}

int readHeaderP7(FILE *fh, struct HeaderInfo *headerData)
{
    int width = -1, height = -1, depth = -1, maxColor = -1, RGB = 0, RGB_Alpha = 0;

    char c[100];
    fscanf(fh, "%s", c);

    while(strcmp("ENDHDR", c))
    {
        if(c[0] == '#')
        {
            while(fgetc(fh) != '\n');
        }
        else if(!strcmp("HEIGHT", c))
        {

            height = getNextInt(fh);
        }
        else if(!strcmp("WIDTH", c))
        {
            width = getNextInt(fh);
        }
        else if(!strcmp("DEPTH", c))
        {
            depth = getNextInt(fh);
        }
        else if(!strcmp("MAXVAL", c))
        {
            maxColor = getNextInt(fh);
        }
        else if(!strcmp("TUPLTYPE", c))
        {
            fscanf(fh, "%s", c);
            if(!strcmp("RGB", c))
            {
                RGB = 1;
            }
            else if(!strcmp("RGB_ALPHA", c))
            {
                RGB_Alpha = 1;
            }
            else
            {
                fprintf(stderr, "Invalid TUPLTYPE: %s\n", c);
                return -1;
            }
        }

        else
        {
            fprintf(stderr, "PPM7 file header is invalid! String %s is not valid!\n", c);
            return -1;
        }
        fscanf(fh, "%s", c);
    }

    headerData->width = width;
    headerData->height = height;
    headerData->depth = depth;
    headerData->maxColor = maxColor;

    printf("Width: %d\n", width);
    printf("Height: %d\n", height);
    printf("Color: %d\n", maxColor);
    printf("Depth: %d\n", depth);

    if(width <= 0 || height <= 0)
    {
        printf("Invalid file format - width, or height.\n");
        return -1;
    }

    if(maxColor > 255 || maxColor <= 0)
    {
        printf("Invalid file format - max color value is not outside appropriate values.\n");
        return -1;
    }

    if(depth == 3 && RGB != 1)
    {
        fprintf(stderr, "PPM7 file header is invalid! Depth is 3, but TUPLETYPE is not RGB!\n");
        return -1;
    }
    else if(depth == 4 && RGB_Alpha != 1)
    {
        fprintf(stderr, "PPM7 file header is invalid! Depth is 4, but TUPLETYPE is not RGB_ALPHA!\n");
        return -1;
    }

    // Skip to the end of the header to start reading the body if done validating the fields:
    while(fgetc(fh) != '\n');

    return 0;
}

int verifyPPMVersion(int ppm_version)
{
    return ppm_version == '3' || ppm_version == '6' || ppm_version == '7';
}

int colorInBounds(int maxValue, int currColorVal)
{
    return 0 <= currColorVal && currColorVal <= maxValue;
}

void skipComments(FILE *fh)
{
    if(fgetc(fh) == '#')
    {
        while(fgetc(fh) != '\n');
    }
}

int getNextInt(FILE *fh)
{
    int nextInt = -1, c = 0;

    while(!feof(fh) && nextInt == -1)
    {
        c = fgetc(fh);
        if(c == '#') 
            while (fgetc(fh) != '\n');

        if(isdigit(c))
        {
            fseek(fh, -1, SEEK_CUR);
            fscanf(fh, "%d", &nextInt);
        }
    }

    return nextInt;
}

int getNextPixel(FILE *fh)
{
    int nextInt = -1, c = 0;

    while(!feof(fh) && nextInt == -1)
    {
        c = fgetc(fh);

        if(isdigit(c))
        {
            fseek(fh, -1, SEEK_CUR);
            fscanf(fh, "%d", &nextInt);
        }
    }

    return nextInt;
}

int validateOutputPath(FILE *fh, char *outputFilename)
{
    if(!fh)
    {
        fprintf(stderr, "Invalid output path: %s\n", outputFilename);
        return -1;
    }
    return 0;
}

int convertRGBAToRGB(struct RGBAPixel *rgbaArray, struct RGBPixel *rgbArray, struct HeaderInfo *headerData)
{
    int totalPixels = headerData->height * headerData->width;

    for(int i = 0; i < totalPixels; i++)
    {
        rgbArray[i].red = rgbaArray[i].red;
        rgbArray[i].green = rgbaArray[i].blue;
        rgbArray[i].blue = rgbaArray[i].green;
    }

    free(rgbaArray);
    return 0;
}

int verifyTupleType(FILE *fh, int depth)
{
    char *tupleStr;
    switch(depth)
    {
        case 3: tupleStr = "RGB"; break;
        case 4: tupleStr = "RGB_ALPHA"; break;
        default: printf("Invalid depth.\n"); return -1;
    }

    for(int i = 0; i < strlen(tupleStr); i += 1)
    {
        if(fgetc(fh) != tupleStr[i])
        {
            printf("Malformed TUPLTYPE string!\n");
            return -1;
        }
    }
    return 0;
}