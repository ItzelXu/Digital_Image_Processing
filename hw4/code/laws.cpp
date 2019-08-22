//(1) Name: Yifei Liu
//(2) USC ID Number: 3852294243 
//(3) USC Email: liu534@usc.edu
//(4) Submission Date: 3/19/2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <queue>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/ml/ml.hpp> 

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


	////////////////////////// END CODE ////////////////////////////////////////////////

	//1. Expand the image matrix.

	cout << "1. Expand the image matrix." <<endl;
	int N = 5; //Window size


	double Imagedata_Expassion[Row + N - 1][Column + N - 1][BytesPerPixel];

	//fill inside

	for (int k = 0; k < BytesPerPixel; k++){

		for (int i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for (int j = (N - 1)/2; j < Column + (N - 1)/2; j++){

				Imagedata_Expassion[i][j][k] = Imagedata[i - (N - 1)/2][j - (N - 1)/2][k];
			}
		}
	}
	cout << "Fill inside succeed!" <<endl;

	//fill the expended column.

	for (int k = 0; k < BytesPerPixel; k++){

		for (int i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for (int j = 0; j < (N - 1)/2 ; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[i][N - 1 - j][k];

			}
			for (int j = (N - 1)/2 + Column; j < N - 1 + Column; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[i][N - 3 + 2 * Column - j][k];
			}
		}
	}
	cout << "Fill the expended column succeed!" <<endl;
	//fill the expeneded row.

	for(int k = 0; k < BytesPerPixel; k++){

		for (int i = 0; i < (N - 1)/2 ; i++){
			for(int j = 0; j < Column + N - 1; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[N - 1 - i][j][k];
			}
		}

		for (int i = (N - 1)/2 + Row; i < N - 1 + Row; i++){
			for(int j = 0; j < Column + N - 1; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Expassion[N - 3 + 2 * Row - i][j][k];
			}
		}
	}



	cout << "Fill the expended row succeed!" <<endl;
	
	//prepare
	double image_sum = 0.0;
	for (int i = 0; i < Row + N - 1; i++){
		for (int j = 0; j < Column + N - 1; j++){
			Imagedata_Expassion[i][j][0] = Imagedata_Expassion[i][j][0] / 255;
			image_sum += Imagedata_Expassion[i][j][0];
		}
	}
	double image_mean = image_sum / ((Row + N - 1) * (Column + N - 1));
	cout << "image_mean: " << image_mean << endl;
	
	for (int i = 0; i < Row + N - 1; i++){
		for (int j = 0; j < Column + N - 1; j++){
			Imagedata_Expassion[i][j][0] = Imagedata_Expassion[i][j][0] - image_mean;
		}
	}

	
	
	//Law's filters
	
//	int L[5] = [ 1, 4,6, 4, 1];
//	int E[5] = [-1,-2,0, 2, 1];
//	int S[5] = [-1, 0,2, 0,-1];
//	int W[5] = [-1, 2,0,-2, 1];
//	int R[5] = [ 1,-4,6,-4, 1];
	int current[5][5] = {0};
	double Imagedata_filted[Row][Column][1];
//	double sum = {0.0};
	double square_sum[25] = {0.0};
	int num = 0;
//	double sum = 0;
	double mean;
	int Laws[5][5] = {{ 1, 4,6, 4, 1},{-1,-2,0, 2, 1},{-1, 0,2, 0,-1},{-1, 2,0,-2, 1},{ 1,-4,6,-4, 1}};
	for(int a = 0; a < N; a++){
		for(int b = 0; b < N; b++){
			for (int k = 0; k < N; k++){
				for (int l = 0; l < N; l++){
				current[k][l] = Laws[a][k] * Laws[b][l];
				cout << current[k][l] << "\t";
				}
				cout << "\n";
			}
			cout << "\n";
//			sum = 0.0;
			for (int i = (N - 1)/2; i < Row + (N - 1)/2; i++){
				for(int j = (N - 1)/2; j < Column + (N - 1)/2; j++){
//					Imagedata_filted[i - (N - 1)/2][j - (N - 1)/2][0] = 0;
					double temp = 0;
					for (int m = 0; m < N; m++){
						for(int n = 0; n < N; n++){
//							cout << current[m][n] << endl;
//							cout << Imagedata_Expassion[i - (N - 1)/2 + m][j - (N - 1)/2 + n][0] << endl;
							temp += Imagedata_Expassion[i - (N - 1)/2 + m][j - (N - 1)/2 + n][0] * current[m][n];
//							cout << temp << endl;
						}
					}
					Imagedata_filted[i - (N - 1)/2][j - (N - 1)/2][0] = temp;
//					cout << i  - (N - 1)/2 << ", " << j - (N - 1)/2 << ": " << Imagedata_filted[i - (N - 1)/2][j - (N - 1)/2][0] <<endl;
//					sum += abs(temp);
					square_sum[num] += Imagedata_filted[i - (N - 1)/2][j - (N - 1)/2][0] * Imagedata_filted[i - (N - 1)/2][j - (N - 1)/2][0];
					
				}
			}


			cout << "square sum: " << square_sum[num]<<endl;
//			cout << count <<endl;
			num = num + 1;
		}
	}
	
//	normalize vector
//	double sum_sum;
//	for(int i = 0; i < 25; i++){
//		sum_sum += sum[i];
//	}
//	int mean = sum_sum / (Row * Column)
	
	for(int i = 0; i < 25; i++){
		square_sum[i] = sqrt(square_sum[i] / (Row * Column));
		cout<<square_sum[i]<<endl;
	}
	
	// functions for output array to be plot
	FILE *data_f = fopen("square_sum_2.txt", "w");
	if (data_f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	for (int i = 0;i < 25; i++) {
		fprintf(data_f, "%f\n", square_sum[i]);
	}
	fclose(data_f);
	

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[5] << endl;
		exit(1);
	}
	fwrite(Imagedata_filted, sizeof(unsigned char), (Row)*(Column)*1, file);
	fclose(file);

	return 0;
}