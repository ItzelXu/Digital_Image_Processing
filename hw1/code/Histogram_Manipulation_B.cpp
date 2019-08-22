// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	//int Size = 256;
	int Row = 400;
	int Column = 400;

	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Row = 400] [Column = 400]" << endl;
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
			//BytesPerPixel = atoi(argv[3]);
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

	cout << "BytesPerPixel: " << BytesPerPixel <<endl;
	cout << "Imagedata size:" << sizeof(Imagedata) <<endl;

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	//Compute histograms
	
	int i = 0;
	int j = 0;
	int k = 0;
	int intensity_value[256] = {0};
	int Integral[256] = {0};
	int Row_count[Row*Column];
	int Column_count[Row*Column];
	int m = 0;
	int n = 0;


	for(i = 0; i < Row; i++){
		for(j = 0; j < Column; j++){

			// cout << "intensity_value: " << i << " /" << j <<" is " << intensity_value[(int)Imagedata[i][j][0]] << endl;
			intensity_value[(int)Imagedata[i][j][0]] = intensity_value[(int)Imagedata[i][j][0]] + 1;
			
		}
	}
	
	
	// for (i = 0; i < 256; i++){
	// 	cout << "intensity_value: " << i << " is " << intensity_value[i] << endl;
	// }
	

	// Accumulation
	for (k = 0; k < 256; k++){
		for (i = 0; i < Row; i++){
			for(j = 0; j < Column; j++){

				if(Imagedata[i][j][0] == k){
					Row_count[m] = i;
					Column_count[m] = j;
					m = m + 1;

				}
			}
		}
	}

	n = Row * Column / 256;

	for (k = 0; k < 256 ; k++){
		for (m = k * n; m < (k + 1) * n; m++){
			Imagedata[Row_count[m]][Column_count[m]][0] = k;

		}

	}
	
	
	//Caculate the modified histogram
	
	int intensity_value_plus[256] = {0};
	
	for(i = 0; i < Row; i++){
		for(j = 0; j < Column; j++){

			// cout << "intensity_value: " << i << " /" << j <<" is " << intensity_value[(int)Imagedata[i][j][0]] << endl;
			intensity_value_plus[(int)Imagedata[i][j][0]] = intensity_value_plus[(int)Imagedata[i][j][0]] + 1;
			
		}
	}
	
	 // functions for output array to be plot
	 FILE *data_f = fopen("histogram_rosemix_B.txt", "w");
	 if (data_f == NULL)
	 {
	 	printf("Error opening file!\n");
	 	exit(1);
	 }
	 for (i = 0;i < 256; i++) {
	 	fprintf(data_f, "%d\n", intensity_value_plus[i]);
	 }
	 fclose(data_f);



	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Row*Column*1, file);
	fclose(file);

	cout << "Write image data succeed! " <<endl; 

	return 0;
}
