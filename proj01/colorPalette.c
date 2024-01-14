/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

//You don't need to call this function but it helps you understand how the arguments are passed in 
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	int * colorcount = (int * ) malloc(sizeof(int));
	uint8_t ** colors = FileToColorMap(colorfile, colorcount);
	FILE * outputfile_ptr = fopen(outputfile, "w");
	fprintf(outputfile_ptr, "P3 %d %d %d\n", width, heightpercolor*(*colorcount), 255);
	for (int _ = 0; _ < (*colorcount); ++_) {
		uint8_t red = colors[_][0];
		uint8_t green = colors[_][1];
		uint8_t blue = colors[_][2];
		for (int i = 0; i < heightpercolor; ++i) {
			for (int j = 0; j < width; ++j) {
				fprintf(outputfile_ptr, "%d %d %d", red, green, blue);
				if (j == width - 1) {
					fprintf(outputfile_ptr, "\n");
				} else {
					fprintf(outputfile_ptr, " ");
				}
			}
		}
	}
	fclose(outputfile_ptr);
	free(colorcount);
	free(colors);
	return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	int * colorcount = (int *) malloc(sizeof(int));
	uint8_t ** colors = FileToColorMap(colorfile, colorcount);
	FILE * outputfile_ptr = fopen(outputfile, "w");
	fprintf(outputfile_ptr, "P6 %d %d %d\n", width, heightpercolor*(*colorcount), 255);
	for (int _ = 0; _ < (*colorcount); ++_) {
		uint8_t red = colors[_][0];
		uint8_t green = colors[_][1];
		uint8_t blue = colors[_][2];
		uint8_t color[3] = {red, green, blue};
		for (int i = 0; i < heightpercolor; ++i) {
			for (int j = 0; j < width; ++j) {
				fwrite(color, sizeof(uint8_t), sizeof(color), outputfile_ptr);
			}
		}
	}
	fclose(outputfile_ptr);
	free(colorcount);
	free(colors);
	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)+1];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}




