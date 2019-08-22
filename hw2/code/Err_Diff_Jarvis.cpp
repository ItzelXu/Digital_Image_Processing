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
	
	
	//1. Expand the image matrix.----------------------------------------------------------
	
	cout << "1.Expend the matrix" <<endl;


	int N = 5; //Window size
	int i,j,k,m,n;



	double Imagedata_Expassion[Row + N - 1][Column + N - 1][1];

	//fill inside

	for (k = 0; k < 1; k++){

		for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for (j = (N - 1)/2; j < Column + (N - 1)/2; j++){

				Imagedata_Expassion[i][j][k] = Imagedata[i - (N - 1)/2][j - (N - 1)/2][k];
			}
		}
	}
	cout << "Fill inside succeed!" <<endl;

	//fill the expended column.

	for (k = 0; k < 1; k++){

		for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for (j = 0; j < (N - 1)/2 ; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[i][N - 1 - j][k];

			}
			for (j = (N - 1)/2 + Column; j < N - 1 + Column; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[i][N - 3 + 2 * Column - j][k];
			}
		}
	}
	cout << "Fill the expended column succeed!" <<endl;
	//fill the expeneded row.

	for(k = 0; k < 1; k++){

		for (i = 0; i < (N - 1)/2 ; i++){
			for(j = 0; j < Column + N - 1; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[N - 1 -i][j][k];
			}
		}

		for (i = (N - 1)/2 + Row; i < N - 1 + Row; i++){
			for(j = 0; j < Column + N - 1; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[N - 3 + 2 * Row - i][j][k];
			}
		}
	}

	cout << "Fill the expended row succeed!" <<endl;
	
	// Set up Floyd error diffusion--------------------------------------------------
	double Jarvis[5][5] = {{0.0,0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0,0.0},{0.0,0.0,0.0,7.0,5.0},{3.0,5.0,7.0,5.0,3.0},{1.0,3.0,5.0,3.0,1.0}};
	double Jarvis_Mirror[5][5] = {{0.0},{0.0},{5.0,7.0,0.0,0.0,0.0},{3.0,5.0,7.0,5.0,3.0},{1.0,3.0,5.0,3.0,1.0}};

	
	
	//set threshold--------------------------------------------------------------------
	int threshold = 88;
	
	//scanning------------------------------------------------------------------------
	unsigned char Imagedata_Diffusion[Row][Column][1];
	double err = 0.0;
	for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
		if (i % 2 == 0){
			for(j = (N - 1)/2; j < Column + (N - 1)/2; j++){
				if (Imagedata_Expassion[i][j][0] < threshold){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 0;
					}
				else{
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 255;
				}
				
				err = Imagedata_Expassion[i][j][0] - Imagedata_Diffusion[i - (N - 1)/2][j - (N - 1)/2][0];
				for (m = 0; m < N; m++){
					for(n = 0;n < N;n++){
						Imagedata_Expassion[i - (N - 1)/2 + m][j- (N - 1)/2 + n][0] += err * Jarvis[m][n] / 48.0;
					}
				}
					
				
			}
		}
		else {
			for(j = Column + (N - 1)/2 - 1; j >= (N - 1)/2; j--){
				if (Imagedata_Expassion[i][j][0] < threshold){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 0;
					}
				else{
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 255;
				}
				
				err = Imagedata_Expassion[i][j][0] - Imagedata_Diffusion[i - (N - 1)/2][j - (N - 1)/2][0];
				for (m = 0; m < N; m++){
					for(n = 0;n < N;n++){
						Imagedata_Expassion[i - (N - 1)/2 + m][j- (N - 1)/2 + n][0] += err * Jarvis_Mirror[m][n] / 48.0;
					}
				}
					
				

			}
		}
		
	}


	////////////////////////// END CODE ////////////////////////////////////////////////


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_Diffusion, sizeof(unsigned char), Row*Column*1, file);
	fclose(file);

	return 0;
}
