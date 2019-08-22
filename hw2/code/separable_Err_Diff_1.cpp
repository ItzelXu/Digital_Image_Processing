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

	
//	// Check for proper syntax
//	if (argc < 3){
//		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
//		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Row = 300] [Column = 390]" << endl;
//		return 0;
//	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 3; // default is grey image
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
	
	//Convert RGB -> CMYK
	unsigned char Imagedata_CMYK[Row][Column][BytesPerPixel];
	int R,G,B;
	double C,M,Y;
	int i,j,k,m,n;
	float W;

	for(i = 0; i < Row; i++){
		for(j = 0 ;j < Column; j++){
			R = Imagedata[i][j][0];
			G = Imagedata[i][j][1];
			B = Imagedata[i][j][2];
			
			C = (255 -  R);
			M = (255 -  G);
			Y = (255 -  B);

			Imagedata_CMYK[i][j][0] = C;
			Imagedata_CMYK[i][j][1] = M;
			Imagedata_CMYK[i][j][2] = Y;
//			cout << C << " "<< M <<" "<< " "<< Y <<endl;
			
		}
	}
	
	cout<<" RGB->CMYK succeed"<<endl;


	
	
	
	//1. Expand the image matrix.----------------------------------------------------------
	
	cout << "1.Expend the matrix" <<endl;


	int N = 3; //Window size



	double Imagedata_Expassion[Row + N - 1][Column + N - 1][BytesPerPixel];

	//fill inside

	for (k = 0; k < BytesPerPixel; k++){
		for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for (j = (N - 1)/2; j < Column + (N - 1)/2; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_CMYK[i - (N - 1)/2][j - (N - 1)/2][k];
			}
		}
	}
	cout << "Fill inside succeed!" <<endl;

	//fill the expended column.

	for (k = 0; k < BytesPerPixel; k++){

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

	for(k = 0; k < BytesPerPixel; k++){

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
	double Floyd[3][3] = {{0,0,0},{0,0,7.0},{3.0,5.0,1.0}};
	double Floyd_Mirror[3][3] = {{0,0,0},{7.0,0,0.0},{1.0,5.0,3.0}};
	
	
	//set threshold--------------------------------------------------------------------
	double threshold = 100;
	
	//scanning------------------------------------------------------------------------
	unsigned char Imagedata_Diffusion[Row][Column][BytesPerPixel];
	double err = 0.0;
	for(k = 0; k < BytesPerPixel;k++){
		for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
				if (i % 2 == 1){
					for(j = (N - 1)/2; j < Column + (N - 1)/2; j++){
						if (Imagedata_Expassion[i][j][k] < threshold){
							Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][k] = 0;
							}
						else{
							Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][k] = 255;
						}
						
						err = Imagedata_Expassion[i][j][k] - Imagedata_Diffusion[i - (N - 1)/2][j - (N - 1)/2][k];
						for (m = 0; m < 3; m++){
							for(n = 0;n < 3;n++){
								Imagedata_Expassion[i - (N - 1)/2 + m][j- (N - 1)/2 + n][k] += err * Floyd[m][n] / 16.0;
							}
						}
							
						
					}
				}
				else {
					for(j = Column + (N - 1)/2 - 1; j >= (N - 1)/2; j--){
						if (Imagedata_Expassion[i][j][k] < threshold){
							Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][k] = 0;
							}
						else{
							Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][k] = 255;
						}
						
						err = Imagedata_Expassion[i][j][k] - Imagedata_Diffusion[i - (N - 1)/2][j - (N - 1)/2][k];
						for (m = 0; m < 3; m++){
							for(n = 0;n < 3;n++){
								Imagedata_Expassion[i - (N - 1)/2 + m][j- (N - 1)/2 + n][k] += err * Floyd_Mirror[m][n] / 16.0;
							}
						}
							
						

					}
				}
				
			}
	}
	
	//Convert CMY to RGB
	
	for(k = 0; k < BytesPerPixel; k++){
		for(i = 0; i < Row; i++){
			for(j = 0; j < Column; j++){
				Imagedata_Diffusion[i][j][k] = 255 - Imagedata_Diffusion[i][j][k];
				
			}
		}
	}
			


	////////////////////////// END CODE ////////////////////////////////////////////////


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_Diffusion, sizeof(unsigned char), Row*Column*BytesPerPixel, file);
	fclose(file);

	return 0;
}
