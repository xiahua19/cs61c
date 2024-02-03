/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
	FILE * colorfile_ptr = fopen(colorfile, "r");
	if (colorfile_ptr == NULL) {
		fclose(colorfile_ptr);
		return NULL;
	}

	int columns = 3;
	fscanf(colorfile_ptr, "%d", colorcount);
	
	uint8_t ** colors = (uint8_t **) malloc ((*colorcount) * sizeof(uint8_t *));
	uint8_t * data = (uint8_t *) malloc(columns * (*colorcount) * sizeof(uint8_t));
	for (int i = 0; i < (*colorcount); ++i) {
		colors[i] = data + i * columns;
	}

	for (int i = 0; i < (*colorcount); ++i) {
		for (int j = 0; j < columns; ++j) {
			fscanf(colorfile_ptr, "%hhd", &colors[i][j]);
		}
	}
	fclose(colorfile_ptr);
	return colors;
}

// int main() {
// 	char colorfile[20] = "defaultcolormap.txt";
// 	int columns = 3;
// 	int * colorcount = malloc(sizeof(int));
// 	uint8_t** colors = FileToColorMap(colorfile, colorcount);
// 	printf("%d\n", (*colorcount));
// 	for (int i = 0; i < (*colorcount); ++i) {
// 		for (int j = 0; j < columns; ++j) {
// 			printf("%d ", colors[i][j]);
// 		}
// 		printf("\n");
// 	}
// }