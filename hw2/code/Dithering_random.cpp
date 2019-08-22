			//(1) Name: Yifei Liu
			//(2) USC ID Number: 3852294243 
			//(3) USC Email: liu534@usc.edu
			//(4) Submission Date: 1/22/2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	//int Size = 256;
	int Row = 400;
	int Column = 600;

	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Row = 300] [Column = 390]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
//			Size = atoi(argv[4]);
			Row = atoi(argv[4]);
			Column = atoi(argv[5]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata[Row][Column][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Row*Column*BytesPerPixel, file);
	fclose(file);
	cout << "Image read succeed!" <<endl;

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	int i,j;
	unsigned char Imagedata_output[Row][Column][1];
	int rand_num;
	for(i = 0; i < Row; i++){
		for(j = 0;j < Column; j++){
			
			rand_num = rand()%256;
			if (Imagedata[i][j][0] > rand_num){
				Imagedata_output[i][j][0] = 255;
			}
			else{
				Imagedata_output[i][j][0] = 0;
			}
		}
		
	}
	
	
	////////////////////////// END CODE ////////////////////////////////////////////////


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_output, sizeof(unsigned char), Row*Column*1, file);
	fclose(file);

	return 0;
}
