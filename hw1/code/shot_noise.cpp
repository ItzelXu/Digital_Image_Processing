//(1) Name: Yifei Liu
//(2) USC ID Number: 3852294243 
//(3) USC Email: liu534@usc.edu
//(4) Submission Date: 1/22/2019
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

double convert(float num){

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
		//cout << "program_name 1.input_image.raw 2.ori_image.raw 3.output_image.raw (biased) 4.output_image.raw (unbiased) 5.[BytesPerPixel = 1] 6.[Row = 256] 7.[Column = 256]" << endl;

		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 5){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[5]);
		// Check if size is specified
		if (argc >= 7){
//			Size = atoi(argv[4]);
			//BytesPerPixel = atoi(argv[3]);
			Row = atoi(argv[6]);
			Column = atoi(argv[7]);
		}
	}
	cout << "Row and Column: " << Row << " and" << Column <<endl;
	
	// Allocate image data array
	unsigned char Imagedata[Row][Column][BytesPerPixel];
	unsigned char Imagedata_ori[Row][Column][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Row*Column*BytesPerPixel, file);
	fclose(file);
	cout << "Image read succeed!" <<endl;


	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata_ori, sizeof(unsigned char), Row*Column*BytesPerPixel, file);
	fclose(file);

	cout << "BytesPerPixel: " << BytesPerPixel <<endl;
	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	int N = 3; //Window size
	int i = 0;
	int j = 0;
	int l = 0;
	int k = 0;
	int ch = 0;

	
	
	cout << "1. Transform Image" <<endl;

	double Imagedata_Transformed[Row][Column][BytesPerPixel];



	for(ch = 0; ch < BytesPerPixel; ch++){
		for (i = 0; i < Row; i++){
			for (j = 0; j < Column; j++){

				Imagedata_Transformed[i][j][ch] =  2 * sqrt(Imagedata[i][j][ch] + 3.0 / 8.0);
			}
		}
	}

	

	//1. Expand the image matrix.

	cout << "2. Expand the image matrix." <<endl;




	double Imagedata_Expassion[Row + N - 1][Column + N - 1][BytesPerPixel];

	//fill inside

	for (k = 0; k < BytesPerPixel; k++){

		for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for (j = (N - 1)/2; j < Column + (N - 1)/2; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_Transformed[i - (N - 1)/2][j - (N - 1)/2][k];
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




	cout << "3. Set upset up gaussian filter" <<endl;


	double gaussian_filter[N][N];
	double r;
	double sum_p;
	double sigma = 1;

	for (i = - (N - 1)/2; i <= (N - 1)/2; i++){
		for (j = - (N - 1)/2; j <= (N - 1)/2; j++){

			r = i*i + j*j;

			gaussian_filter[i + (N - 1)/2][j + (N - 1)/2] = exp(- r / 2 * sigma * sigma);
			sum_p += gaussian_filter[i + (N - 1)/2][j + (N - 1)/2];

		}
	}

	for (i = 0; i < N; i++){
		for(j = 0; j < N; j++){

			gaussian_filter[i][j] = gaussian_filter[i][j]/sum_p;
			cout << gaussian_filter[i][j]<< " ";

			if( j == N - 1){

			cout << endl;
			} 

		}
	}

	cout << "4. convolution by gaussian filter" <<endl;

	double Imagedata_compute_gaussian[Row][Column][BytesPerPixel];
	double middle = 0.0;
	int m;
	int n;


	for(ch = 0; ch < BytesPerPixel; ch++){
		for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for(j = (N - 1)/2; j < Column + (N - 1)/2; j++){
				middle = 0.0;
				for(m = 0; m < N; m++){	
					for(n = 0; n < N; n++){
						
						middle += Imagedata_Expassion[i - (N - 1)/2 + m][j - (N - 1)/2 + n][ch] * gaussian_filter[m][n];
	
					}
				}
				Imagedata_compute_gaussian[i - (N - 1)/2][j - (N - 1)/2][ch] = convert(middle);

			}
		}
	}




	 cout << "5. Inverse Transform Image (biased)" <<endl;
	 unsigned char Imagedata_output_biased[Row][Column][BytesPerPixel];

	 for(ch = 0; ch < BytesPerPixel; ch++){
	 	for (i = 0; i < Row; i++){
	 		for (j = 0; j < Column; j++){

	 			Imagedata_output_biased[i][j][ch] = pow(Imagedata_compute_gaussian[i][j][ch],2) / 4.0 - 3.0 / 8.0;
	 		}
	 	}
	 }

	cout << "6. Inverse Transform Image (unbiased)" <<endl;
	unsigned char Imagedata_output_unbiased[Row][Column][BytesPerPixel];


	for(ch = 0; ch < BytesPerPixel; ch++){
		for (i = 0; i < Row; i++){
			for (j = 0; j < Column; j++){

				Imagedata_output_unbiased[i][j][ch] = pow(Imagedata_compute_gaussian[i][j][ch],2)/4.0 - 1.0 / 8.0;
			}
		}
	}

		




	//3. Caculate PSNR

	// compute mse
	double temp1 = 0;
	double mse1 = 0;
	double psnr1 = 0;
	double temp2 = 0;
	double mse2 = 0;
	double psnr2 = 0;
	double temp3 = 0;
	double mse3 = 0;
	double psnr3 = 0;



	for(ch = 0; ch < BytesPerPixel; ch++){
		for(i = 0; i < Row; i++){
			for (j = 0; j < Column; j++){
				temp1 = Imagedata[i][j][ch] - Imagedata_ori[i][j][ch];
				mse1 = mse1 + (temp1 * temp1);
	
				temp2 = Imagedata_output_biased[i][j][ch] - Imagedata_ori[i][j][ch];
				mse2 = mse2 + (temp2 * temp2);
				
				temp3 = Imagedata_output_unbiased[i][j][ch] - Imagedata_ori[i][j][ch];
				mse3 = mse3 + (temp3 * temp3);
	
			}
		}
	}

	mse1 = mse1 / (Row * Column * BytesPerPixel);
	mse2 = mse2 / (Row * Column * BytesPerPixel);
	mse3 = mse3 / (Row * Column * BytesPerPixel);

	psnr1 = 10 * log10(255.0 * 255.0 / mse1);
	psnr2 = 10 * log10(255.0 * 255.0 / mse2);
	psnr3 = 10 * log10(255.0 * 255.0 / mse3);


	cout << "psnr of noise image is :" << psnr1 <<endl;
	cout << "psnr of denoised(biased) is :" << psnr2 <<endl;
	cout << "psnr of denoised(unbiased) is :" << psnr2 <<endl;
	
	
//	 // 4. count the frequency of intensities
//	 int noise;
//	 int noise_trans;
//	 int noise_output;
//	 double frequency_shot[256] = {0.0};
//	 double frequency_gau[256] = {0.0};
//	 double frequency_output[256] = {0.0};
//	 for (i = 0; i < Row; i++) {
//	 	for (j = 0;j < Column; j++) {
//	 		noise = Imagedata[i][j][0] - Imagedata_ori[i][j][0];
//			noise_trans = Imagedata_Transformed[i][j][0] - Imagedata_ori[i][j][0];	
//			noise_output = Imagedata_output[i][j][0] - Imagedata_ori[i][j][0];	
//
//	 		//cout<< noise << " ";
//	 		frequency_shot[noise+200] += 1.0;
//	 		frequency_gau[noise_trans+200] += 1.0;
//	 		frequency_output[noise_output+200] += 1.0;
//
//	 	}
//	 }
//
//	 // functions for output array to be plot
//	 FILE *data_f = fopen("shot_noise.txt", "w");
//	 if (data_f == NULL)
//	 {
//	 	printf("Error opening file!\n");
//	 	exit(1);
//	 }
//	 for (i = 0;i < 256; i++) {
//	 	fprintf(data_f, "%f\n", frequency_shot[i] / (Row * Column));
//	 }
//	 fclose(data_f);
//	
//	 // functions for output array to be plot
//	 data_f = fopen("shot_noise_trans.txt", "w");
//	 if (data_f == NULL)
//	 {
//	 	printf("Error opening file!\n");
//	 	exit(1);
//	 }
//	 for (i = 0;i < 256; i++) {
//	 	fprintf(data_f, "%f\n", frequency_gau[i] / (Row * Column));
//	 }
//	 fclose(data_f);
//	
//	 // functions for output array to be plot
//	 data_f = fopen("shot_noise_denoised.txt", "w");
//	 if (data_f == NULL)
//	 {
//	 	printf("Error opening file!\n");
//	 	exit(1);
//	 }
//	 for (i = 0;i < 256; i++) {
//	 	fprintf(data_f, "%f\n", frequency_output[i] / (Row * Column));
//	 }
//	 fclose(data_f);


	////////////////////////// END CODE ////////////////////////////////////////////////


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[3],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(Imagedata_output_biased, sizeof(unsigned char), Row*Column*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen(argv[4],"wb"))) {
		cout << "Cannot open file: " << argv[3] << endl;
		exit(1);
	}
	fwrite(Imagedata_output_unbiased, sizeof(unsigned char), Row*Column*BytesPerPixel, file);
	fclose(file);

	return 0;
}
