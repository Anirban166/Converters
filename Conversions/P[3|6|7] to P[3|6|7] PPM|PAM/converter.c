#include "converter.h"
// Four core "functionalities among the .ppm files:
// 1) Reading of P3 files into RGBA ordered memory. (P3 is ASCII)
// 2) Reading of P6 files into RGBA ordered memory. (P6 is binary)
// 3) Writing of P3 files from RGBA ordered memory.
// 4) Writing of P6 files from RGBA ordered memory.
int main(int argc, char **argv)
{
    if(argc != 4)
    {
        fprintf(stdout, "\nError: Four input parameters (inclusive of the executable name) are expected.\n\nProper usage looks like:\n\n./ExecutableName <MagicNumberForTheDesiredOutputPPMFile> <InputFilename> <OutputFilename>\n\nFor example, it should be something like this:\n./exec 3 PPM7test.pam PPM7to6testOutput.pam\n\nExiting the program!\n");
        exit(-1);
    }
    else fprintf(stdout, "\nThe number of arguments passed is: %d\nThe first argument is: %s\nThe second argument is: %s\nThe third argument is: %s\nThe fourth argument is: %s\n", argc, argv[0], argv[1], argv[2], argv[3]);

    int writeFileType = atoi(argv[1]);
    struct HeaderInfo *headerData = (struct HeaderInfo *)malloc(sizeof(struct HeaderInfo));

    FILE *fh = fopen(argv[2], "rb");
    if(!fh)
    {
        fprintf(stderr, "Input file not found at the path %s.\n", argv[2]);
        return -1;
    }

    char magicNumber;
    int c = fgetc(fh);
    if(c == 'P')
    {
        magicNumber = fgetc(fh);
        fprintf(stdout, "Received magic number from the header data: P%c\n", magicNumber);
        if(!verifyPPMVersion(magicNumber))
        {
            fprintf(stdout, "File format with magic number %c in the header is not supported. Valid versions for the number include 3, 6, and 7.\n", magicNumber);
        }
    }

    else
    {
        fprintf(stdout, "Image is not a PPM type and is not supported.\n");
        return -1;
    }

    struct RGBPixel *rgbArray;
    struct RGBAPixel *rgbaArray;
    if(magicNumber == '3')
    {
        if(readHeaderP3P6(fh, headerData) == -1)
        {
            printf("Error while parsing the header of the .ppm (P3) file.\n");
            return -1;
        }
        rgbArray = malloc(sizeof(struct RGBPixel) * headerData->width * headerData->height);
        headerData->ppmVersion = '3';

        if(readPPM3(fh, rgbArray, headerData) == -1)
        {
            return -1;
        }
    }

    else if(magicNumber == '6')
    {
        if(readHeaderP3P6(fh, headerData) == -1)
        {
            printf("Error while parsing the header of the .ppm (P6) file.\n");
            return -1;
        }
        rgbArray = malloc(sizeof(struct RGBPixel) * headerData->width * headerData->height);
        headerData->ppmVersion = '6';

        if(readPPM6(fh, rgbArray, headerData) == -1)
        {
            return -1;
        }
    }

    else if(magicNumber == '7')
    {
        headerData->ppmVersion = '7';
        if(readHeaderP7(fh, headerData) == -1)
        {
            printf("Error while parsing the header of the .pam (P7) file. \n");
            return -1;
        }

        int depth = headerData->depth;
        if(depth == 4)
        {
            rgbaArray = malloc(sizeof(struct RGBAPixel) * headerData->width * headerData->height);
            headerData->depth = 4;
            if (readPPM7(fh, rgbaArray, headerData) == -1)
            {
                return -1;
            }
        }
        else
        {
            rgbArray = malloc(sizeof(struct RGBPixel) * headerData->width * headerData->height);
            if(readPPM6(fh, rgbArray, headerData) == -1) // Since the body of the file is exactly the same as a P6 ppm file.
            {
                return -1;
            }
        }
    }
    else
    {
        printf("Invalid file type (check magic number and the file name along with the extension) provided as input. Supported types include P3 or P6 .ppm files and .pam (P7) files.\n Exiting.");
        return 1;
    }
    fclose(fh);

    if(writeFileType == 3)
    {
        if(magicNumber == '7' && headerData->depth == 4)
        {
            rgbArray = malloc(sizeof(struct RGBPixel) * headerData->width * headerData->height);
            convertRGBAToRGB(rgbaArray, rgbArray, headerData);
        }
        if(writePPM3(rgbArray, headerData, argv[3]) == -1)
        {
            return -1;
        }
    }

    else if(writeFileType == 6)
    {
        if(magicNumber == '7' && headerData->depth == 4)
        {
            printf("Converting RGBA to RGB!\n");
            rgbArray = malloc(sizeof(struct RGBPixel) * headerData->width * headerData->height);
            convertRGBAToRGB(rgbaArray, rgbArray, headerData);
        }
        if(writePPM6(rgbArray, headerData, argv[3]) == -1)
        {
            return -1;
        }
    }

    else if(writeFileType == 7)
    {
        if(headerData->depth == 3)
        {
            if(writePPM7RGB(rgbArray, headerData, argv[3]))
            {
                return -1;
            }
        }
        else
        {
            if(writePPM7RGBAlpha(rgbaArray, headerData, argv[3]))
            {
                return -1;
            }
        }
    }
    else
    {
        printf("Invalid file type (check magic number and the file name along with the extension) for the file to be outputted. Supported types include P3 or P6 .ppm files and .pam (P7) files.\n Exiting.");
        return 1;
    }

    if(writeFileType == 7 && headerData->depth == 4)
    {
        free(rgbaArray);
    }
    else
    {
        free(rgbArray);
    }
    free(headerData);

    return 0;
}
