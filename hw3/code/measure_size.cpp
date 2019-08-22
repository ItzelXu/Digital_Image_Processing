//(1) Name: Yifei Liu
//(2) USC ID Number: 3852294243 
//(3) USC Email: liu534@usc.edu
//(4) Submission Date: 1/22/2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <queue>

using namespace std;

int convert(float num){

	if(num > 255){
		num = 255;
	}

	if(num < 0){
		num = 0;
	}
	return num;
}


int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	//int Size = 256;
	int Row;
	int Column;

	
	// Check for proper syntax
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw ori_image.raw output_image.raw [BytesPerPixel = 1] [Row = 256] [Column = 256]" << endl;
		//cout << "program_name 1.input_image.raw 2.ori_image.raw 3.output_image.raw 4.[BytesPerPixel = 1] 5.[Row = 256] 6.[Column = 256]" << endl;

		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 3){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 3){
//			Size = atoi(argv[4]);
			//BytesPerPixel = atoi(argv[3]);
			Row = atoi(argv[4]);
			Column = atoi(argv[5]);
		}
	}
	cout << "Row and Column: " << Row << " and " << Column <<endl;
	cout << "Bytebypixels: " << BytesPerPixel <<endl;
	
	// Allocate image data array
	unsigned char Imagedata[Row][Column][BytesPerPixel];



	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Row*Column*BytesPerPixel, file);
	fclose(file);
	cout << "Target Image read succeed!" <<endl;


	
	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	
	// Turn into grayscale
	int Imagedata_Gray[Row][Column][1];
	for (int i = 0; i < Row; i++){
		for(int j = 0; j < Column; j++){
			Imagedata_Gray[i][j][0] = 0.2989 * Imagedata[i][j][0] + 0.5870 * Imagedata[i][j][1] + 0.1140 * Imagedata[i][j][2];
		}
	}
	
	int i = 0;
	int j = 0;
	int intensity_value[256] = {0};
	int Integral[256] = {0};


	for(i = 0; i < Row; i++){
		for(j = 0; j < Column; j++){

			// cout << "intensity_value: " << i << " /" << j <<" is " << intensity_value[(int)Imagedata[i][j][0]] << endl;
			intensity_value[(int)Imagedata_Gray[i][j][0]] = intensity_value[(int)Imagedata_Gray[i][j][0]] + 1;
			
		}
	}
	
	 // functions for output array to be plot
	 FILE *data_f = fopen("histogram_rice.txt", "w");
	 if (data_f == NULL)
	 {
	 	printf("Error opening file!\n");
	 	exit(1);
	 }
	 for (i = 0;i < 256; i++) {
	 	fprintf(data_f, "%d\n", intensity_value[i]);
	 }
	 fclose(data_f);

	
//	Tune to Binary Image
	double Threshold_1 = 70;
	double Threshold_2 = 73;
	
	int Imagedata_N[Row][Column][1];
	for (int i = 0; i < Row; i++){
		for (int j = 0; j < Column; j++){
			if (Imagedata_Gray[i][j][0] > Threshold_1 && Imagedata_Gray[i][j][0] < Threshold_2){
				Imagedata_N[i][j][0] = 0;
			}
			else{
				Imagedata_N[i][j][0] = 1;
			}
		}
	}
	
	// erasure the noise
	for (int i = 0; i < Row; i++){
		for (int j = 0; j < Column; j++){
			if (Imagedata_N[i][j][0] == 1){
				long int compare = 10000000*Imagedata_N[i][j+1][0] + 1000000*Imagedata_N[i-1][j+1][0] + 100000*Imagedata_N[i-1][j][0] + 10000*Imagedata_N[i-1][j-1][0] + 1000*Imagedata_N[i][j-1][0] + 100*Imagedata_N[i+1][j-1][0] + 10*Imagedata_N[i+1][j][0] + Imagedata_N[i+1][j+1][0];
				if(!(compare^0) || !(compare^10) || !(compare^1000) || !(compare^100000) || !(compare^10000000)){
					Imagedata_N[i][j][0] = 0;
				}
			}
		}
	}
	
	int Imagedata_rank = 0;
	int rank_4 = 0;
	int rank_8 = 0;
	// rank the pixel
	for(int iter = 1; iter < 20; iter ++){
		for (int i = 0; i < Row; i++){
			for (int j = 0; j < Column; j++){
				int compare_4 = 1000*Imagedata_N[i][j+1][0] + 100*Imagedata_N[i-1][j][0] + 10*Imagedata_N[i][j-1][0] + 1*Imagedata_N[i+1][j][0];
				int compare_8 = 1000*Imagedata_N[i-1][j+1][0] + 100*Imagedata_N[i-1][j-1][0] + 10*Imagedata_N[i+1][j-1][0] + 1*Imagedata_N[i+1][j+1][0];			
				if(compare_4 == 1000 || compare_4 == 100 || compare_4 == 10 || compare_4 == 1){
					rank_4 = 1;
				}
				else if(compare_4 == 1100 || compare_4 == 1010 || compare_4 == 1001 || compare_4 == 101 || compare_4 == 110 || compare_4 == 11){
					rank_4 = 2;
				}
				else if(compare_4 == 1110 || compare_4 == 1011 || compare_4 == 1101 || compare_4 == 111){
					rank_4 = 3;
				}
				else if(compare_4 == 1111){
					rank_4 = 4;
				}
				else{
					rank_4 = 0;
				}
				
				if(compare_8 == 1111){
					rank_8 = 4;
				}
				else if (compare_8 == 1000 || compare_8 == 100 || compare_8 == 10 || compare_8 == 1){
					rank_8 = 1;
				}
				else if(compare_8 == 1100 || compare_8 == 1010 || compare_8 == 1001 || compare_8 == 101 || compare_8 == 110 || compare_8 == 11){
					rank_8 = 2;
				}
				else if(compare_8 == 1110 || compare_8 == 1011 || compare_8 == 1101 || compare_8 == 111){
					rank_8 = 3;
				}
				else{
					rank_8 = 0;
				}
				
				Imagedata_rank = rank_4 * 2 + rank_8 * 1;
	//			cout <<Imagedata_rank <<endl;
				
				if ((Imagedata_rank < 6) && (Imagedata_N[i][j][0] == 1)){
					Imagedata_N[i][j][0] = 0;
				}
				
				if((Imagedata_rank > 11) && (Imagedata_N[i][j][0] == 0)){
					Imagedata_N[i][j][0] = 1;
				}
			}
		}
	}
	
	
	
	
		
	
//	output
	unsigned char Imagedata_output[Row][Column][1];
	for(int i = 0; i < Row; i++){
		for(int j = 0; j < Column; j++){
			Imagedata_output[i][j][0] = 255 * Imagedata_N[i][j][0];
		}
	}
	

	
	

	
	
	
	
	
	////////////////////////// END CODE ////////////////////////////////////////////////


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[5] << endl;
		exit(1);
	}
	fwrite(Imagedata_output, sizeof(unsigned char), Row*Column*1, file);
	fclose(file);

	return 0;
}
