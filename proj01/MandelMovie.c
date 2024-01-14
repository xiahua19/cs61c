/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
	double log_par = 1 / (framecount-1) * log (finalscale / initialscale);
	for (int i = 0; i < framecount; ++i) {
		double frame_scale = initialscale * pow(exp(log_par), i);
		u_int64_t * frame_output = (uint64_t *) malloc((2*resolution+1) * (2*resolution+1) * sizeof(u_int64_t));
		Mandelbrot(threshold, max_iterations, center, frame_scale, resolution, frame_output);
		output[i] = frame_output;
	}
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function: 
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order. 

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/
	if (argc != 11) {
		printf("%s: Wrong number of arguments, expecting 10\n", argv[0]);
		printUsage(argv);
		return 1;
	}
	double threshold, initialscale, finalscale;
	int framecount;
	u_int64_t max_iterations, resolution;
	ComplexNumber * center;
	char * outputfolder; 
	char * colorfile;

	threshold = atof(argv[1]);
	max_iterations = (u_int64_t) atoi(argv[2]);
	center = newComplexNumber(atof(argv[3]), atof(argv[4]));
	initialscale = atof(argv[5]);
	finalscale = atof(argv[6]);
	framecount = atoi(argv[7]);
	resolution = (u_int64_t) atoi(argv[8]);
	outputfolder = argv[9];
	colorfile = argv[10];
	u_int64_t size = 2 * resolution + 1;
	//END STEP 1
	
	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/
	u_int64_t ** output = (u_int64_t **) malloc(framecount * sizeof(u_int64_t *));
	if (output == NULL) {
		printf("Unable to allocate %lu bytes\n", framecount * sizeof(u_int64_t *));
		return 1;
	}
	u_int64_t * data = (u_int64_t *) malloc(framecount * size * size * sizeof(u_int64_t));
	if (data == NULL) {
		printf("Unable to allocate %lu bytes\n", framecount * size * size * sizeof(u_int64_t));
		return 1;
	}
	for (int i = 0; i < framecount; ++i) {
		output[i] = data + i * size * size;
	}
	
	MandelMovie(threshold, max_iterations, center, initialscale, finalscale, framecount, resolution, output);
	//END STEP 2 

	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/
	int * colorcount = malloc(sizeof(int));
	uint8_t** colors = FileToColorMap(colorfile, colorcount);

	for (int frame = 0; frame < framecount; ++frame) {
		char folder[50];
		strcpy(folder, outputfolder);
		char file_name[20];
		sprintf(file_name, "/frame%05d.ppm", frame);
		strcat(folder, file_name);
		FILE * outputfile_ptr = fopen(folder, "w");
		fprintf(outputfile_ptr, "P6 %ld %ld %d\n", size, size, 255);
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				u_int64_t iter = output[frame][i*size+j];
				if (iter > 0) {
					uint8_t * color = colors[(iter-1)%(*colorcount)];			
					uint8_t red = color[0];
					uint8_t green = color[1];
					uint8_t blue = color[2];
					uint8_t color_list[3] = {red, green, blue};
					fwrite(color_list, sizeof(uint8_t), sizeof(color_list), outputfile_ptr);
				} else if (iter == 0) {
					uint8_t color_list[3] = {0, 0, 0};
					fwrite(color_list, sizeof(uint8_t), sizeof(color_list), outputfile_ptr);
				}
			}
		} 
		fclose(outputfile_ptr);
	}
	//ENDE STEP 3

	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	free(data);
	free(output);
	//END STEP 4

	return 0;
}

