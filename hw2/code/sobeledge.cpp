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
	int Row = 321;
	int Column = 481;

	
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

	// 2. Convert to Grayscale
	
	cout << "2. Convert to Grayscale" <<endl;

	double WB;
	int r,g,b;
	int i,j,k,m,n;
	FILE *data_f;
	double Imagedata_gray[Row][Column][1];
	
	for ( i = 0 ; i < Row; i++){
		for( j = 0; j < Column; j++){
			r = Imagedata[i][j][0];
			g = Imagedata[i][j][1];
			b = Imagedata[i][j][2];
			WB = 0.2989 * r + 0.5870 * g + 0.1140 * b;
//			WB = r/3.0 + g/3.0 + b/3.0;
			Imagedata_gray[i][j][0] = WB;

		}
	}
	
	cout << "Convert to Grayscale succeed!"<<endl;
	
	//3. Sobel Edge Detector
	
	cout << "3.Sobel Edge Detector" <<endl;
	
	int Gr[3][3];
	int Gc[3][3];
	
	
	Gr[0][0] = - 1;
	Gr[1][0] = - 2;
	Gr[2][0] = - 1;
	Gr[0][2] = 1;
	Gr[1][2] = 2;
	Gr[2][2] = 1;
	for (i = 0,j = 1;i < 3;i++){
		Gr[i][j] = 0;
	}
	
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			cout << Gr[i][j]<<' ';
			
		}
		cout << endl;
	}
	
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			
			Gc[i][j] = - Gr[2 - j][i];
		}
	}
	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++){
			cout << Gc[i][j]<<' ';
			
		}
		cout << endl;
	}
	
	
	//4. Expand the image matrix.
	cout << "4.Expend the matrix" <<endl;


	int N = 3; //Window size



	double Imagedata_Expassion[Row + N - 1][Column + N - 1][1];

	//fill inside

	for (k = 0; k < 1; k++){

		for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for (j = (N - 1)/2; j < Column + (N - 1)/2; j++){

				Imagedata_Expassion[i][j][k] = Imagedata_gray[i - (N - 1)/2][j - (N - 1)/2][k];
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
	
	//5. Convolution
	cout << "5. Convolution" <<endl;
	double sum_gradient_row = 0.0;
	double sum_gradient_column = 0.0;
	double gradient_row[Row][Column];
	double gradient_column[Row][Column];
	for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
		for (j = (N - 1)/2; j < Column + (N - 1)/2; j++){
			double dot_product_r = 0.0;
			double dot_product_c = 0.0;
			for(m = 0; m < N; m++){
				for(n = 0; n < N; n++){
					dot_product_r += Imagedata_Expassion[i - (N - 1)/2 + m][j - (N - 1)/2 + n][0] * Gr[m][n];
					dot_product_c += Imagedata_Expassion[i - (N - 1)/2 + m][j - (N - 1)/2 + n][0] * Gc[m][n];
					
				}
			}
			gradient_row[i - (N - 1)/2][j - (N - 1)/2] = abs(dot_product_r/4.0);
			gradient_column[i - (N - 1)/2][j - (N - 1)/2] = abs(dot_product_c/4.0);
					
		}
	}
	
	
	
	//Normalize and sum
	double max_r = 0;
	double max_c = 0;
	double min_r = 20000;
	double min_c = 20000;
	
	for (i = 0; i < Row; i++){
		for(j = 0; j < Column; j++){
			if (max_r < gradient_row[i][j]){
				max_r = gradient_row[i][j];
			}

			if (max_c < gradient_column[i][j]){
				max_c = gradient_column[i][j];
			}

			if (min_r > gradient_row[i][j]){
				min_r = gradient_row[i][j];
			}

			if (min_c > gradient_column[i][j]){
				min_c = gradient_column[i][j];
			}

		}
	}
	
	cout << "max_r:" << max_r<<endl; 
	cout << "max_c:" << max_c<<endl; 
	cout << "min_r:" << min_r<<endl; 
	cout << "min_c:" << min_c<<endl; 
	
	

	double gradient[Row][Column];
	unsigned char gradient_row_output[Row][Column][1];
	unsigned char gradient_col_output[Row][Column][1];
	unsigned char gradient_output[Row][Column][1];
	
	for (i = 0; i < Row; i++){
		for(j = 0;j < Column; j++){
			
			gradient_row[i][j] = (gradient_row[i][j] - min_r)* 255.0 / (max_r - min_r);
			gradient_column[i][j] = (gradient_column[i][j] - min_c) * 255.0 / (max_c - min_c);
			gradient[i][j] = sqrt(pow(gradient_row[i][j],2) + pow(gradient_column[i][j],2));
			
			gradient_row_output[i][j][0] = (int)gradient_row[i][j];
			gradient_col_output[i][j][0] = (int)gradient_column[i][j];
			
		}
		
	}
	double max_g = 0.0;
	double min_g = 10000;
		for (i = 0; i < Row; i++){
			for(j = 0; j < Column; j++){
				if (max_g < gradient[i][j]){
					max_g = gradient[i][j];
				}
				if (min_g > gradient[i][j]){
					min_g = gradient[i][j];
				}
			}
		}
		for(i = 0; i < Row; i++){
			for(j = 0; j < Column; j++){
				gradient[i][j] = (gradient[i][j] - min_g) * 255.0 / (max_g - min_g);
				gradient_output[i][j][0] = (int)gradient[i][j];
			}
		}
		
		
	cout << "Normalize Succeed!"<< endl;
	
	// write gradient file
	if (!(file=fopen("Tiger_grad_r.raw","wb"))) {
		cout << "Cannot open file: " << "Tiger_grad_r.raw" << endl;
		exit(1);
	}
	fwrite(gradient_row_output, sizeof(unsigned char), Row*Column*1, file);
	fclose(file);

	if (!(file=fopen("Tiger_grad_c.raw","wb"))) {
		cout << "Cannot open file: " << "Tiger_grad_r.raw" << endl;
		exit(1);
	}
	fwrite(gradient_col_output, sizeof(unsigned char), Row*Column*1, file);
	fclose(file);
	
	if (!(file=fopen("Tiger_grad.raw","wb"))) {
		cout << "Cannot open file: " << "Tiger_grad.raw" << endl;
		exit(1);
	}
	fwrite(gradient_output, sizeof(unsigned char), Row*Column*1, file);
	fclose(file);
	
	
	
	//Compute the Histogram
	int intensity_value[370] = {0};

	for(i = 0; i < Row; i++){
		for(j = 0; j < Column; j++){

			// cout << "intensity_value: " << i << " /" << j <<" is " << intensity_value[(int)Imagedata[i][j][0]] << endl;
			intensity_value[(int)gradient[i][j]] = intensity_value[(int)gradient[i][j]] + 1;
			
		}
	}
	cout << "Compute Histogram Succeed!"<< endl;


	//Integral
	int Integral[370] = {0};

	for (i = 0; i < 370; i++){
		for (j = 0; j <= i; j++){

			Integral[i] = Integral[i] + intensity_value[j];
		}

	}
	
	cout << "Compute CDF Succeed!"<< endl;

	
	 //functions for output array to be plot
		 data_f = fopen("gradient_row.txt", "w");
		 if (data_f == NULL)
		 {
		 	printf("Error opening file!\n");
		 	exit(1);
		 }
		 for (i = 0;i < Row; i++) {
			for(j = 0; j < Column; j++){
				
				fprintf(data_f, "%f\t", gradient_row[i][j]);

			}
			fprintf(data_f, "\n");
		 }
		 fclose(data_f);
		//
		
		 data_f = fopen("gradient_column.txt", "w");
		 if (data_f == NULL)
		 {
		 	printf("Error opening file!\n");
		 	exit(1);
		 }
		 for (i = 0;i < Row; i++) {
			for(j = 0; j < Column; j++){
				
				fprintf(data_f, "%f\t", gradient_column[i][j]);

			}
			fprintf(data_f, "\n");
		 }
		 fclose(data_f);
		
		 data_f = fopen("gradient_sum.txt", "w");
		 if (data_f == NULL)
		 {
		 	printf("Error opening file!\n");
		 	exit(1);
		 }
		 for (i = 0;i < Row; i++) {
			for(j = 0; j < Column; j++){
				
				fprintf(data_f, "%f\t", gradient[i][j]);

			}
			fprintf(data_f, "\n");
		 }
		 fclose(data_f);
		
		 data_f = fopen("Integralcdf_sobel.txt", "w");
		 if (data_f == NULL)
		 {
		 	printf("Error opening file!\n");
		 	exit(1);
		 }
		for(i = 0; i < 370; i++){
			
			fprintf(data_f, "%d\n", Integral[i]);

		}
		fclose(data_f);
		
		cout << "output Succeed!"<< endl;

		
		//6. Tune the Threshold
	
		cout<< "max_g:" <<max_g<<endl;
					
		cout << "6. Tune the Threshold" <<endl;
		unsigned char Imagedata_edge[Row][Column][1];
		
		double Threshold = 0.30;
		for (i = 0; i < Row; i++){
			for(j = 0; j < Column; j++){
				
				if(gradient[i][j] <= Threshold*255){
					Imagedata_edge[i][j][0] = 255;
				}
				else{
					Imagedata_edge[i][j][0] = 0;
				}
			}
		}

		





	////////////////////////// END CODE ////////////////////////////////////////////////


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_edge, sizeof(unsigned char), Row*Column*1, file);
	fclose(file);

	return 0;
}
