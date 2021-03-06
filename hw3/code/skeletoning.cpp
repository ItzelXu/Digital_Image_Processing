//(1) Name: Yifei Liu
//(2) USC ID Number: 3852294243 
//(3) USC Email: liu534@usc.edu
//(4) Submission Date: 1/22/2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

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
	long int conditional_mask[40] =  {11111011,11111110,10111111,11101111,10100000,101000,1010,10000010,11000001,1110000,11100,111,11110000,11100001,1111000,111100,11110,1111,10000111,11000011,11110001,1111100,11111,11000111,11100011,11111000,111110,10001111,11110011,11100111,11111100,11111001,1111110,111111,10011111,11001111,11110111,11111101,1111111,11011111};
		
	long int unconditional_mask[444] = {1,100,1000000,10000,10,10000000,1000,100000,10100000,101000,10000010,1010,111000,111001,10111000,10111001,1111000,1111001,11111000,11111001,111010,111011,10111010,10111011,1111010,1111011,11111010,11111011,111100,111101,10111100,10111101,1111100,1111101,11111100,11111101,111110,111111,10111110,10111111,1111110,1111111,11111110,11111111,10000011,11000011,10100011,11100011,10000111,11000111,10100111,11100111,10001011,11001011,10101011,11101011,10001111,11001111,10101111,11101111,10010011,11010011,10110011,11110011,10010111,11010111,10110111,11110111,10011011,11011011,10111011,11111011,10011111,11011111,10111111,11111111,101010,101011,10101010,10101011,1101010,1101011,11101010,11101011,101110,101111,10101110,10101111,1101110,1101111,11101110,11101111,111010,111011,10111010,10111011,1111010,1111011,11111010,11111011,111110,111111,10111110,10111111,1111110,1111111,11111110,11111111,10101000,10101001,11101000,11101001,10101010,10101011,11101010,11101011,10101100,10101101,11101100,11101101,10101110,10101111,11101110,11101111,10111000,10111001,11111000,11111001,10111010,10111011,11111010,11111011,10111100,10111101,11111100,11111101,10111110,10111111,11111110,11111111,10100010,10100011,11100010,11100011,10100110,10100111,11100110,11100111,10101010,10101011,11101010,11101011,10101110,10101111,11101110,11101111,10110010,10110011,11110010,11110011,10110110,10110111,11110110,11110111,10111010,10111011,11111010,11111011,10111110,10111111,11111110,11111111,10001010,10001011,11001010,11001011,10101010,10101011,11101010,11101011,10001110,10001111,11001110,11001111,10101110,10101111,11101110,11101111,10011010,10011011,11011010,11011011,10111010,10111011,11111010,11111011,10011110,10011111,11011110,11011111,10111110,10111111,11111110,11111111,10101,10010100,1010100,10010101,1010101,11010100,11010101,10111,10010110,1010110,10010111,1010111,11010110,11010111,110101,10110100,1110100,10110101,1110101,11110100,11110101,110111,10110110,1110110,10110111,1110111,11110110,11110111,11101,10011100,1011100,10011101,1011101,11011100,11011101,11111,10011110,1011110,10011111,1011111,11011110,11011111,111101,10111100,1111100,10111101,1111101,11111100,11111101,111111,10111110,1111110,10111111,1111111,11111110,11111111,1010100,1010010,1010110,1010001,1010101,1010011,1010111,11010100,11010010,11010110,11010001,11010101,11010011,11010111,1110100,1110010,1110110,1110001,1110101,1110011,1110111,11110100,11110010,11110110,11110001,11110101,11110011,11110111,1011100,1011010,1011110,1011001,1011101,1011011,1011111,11011100,11011010,11011110,11011001,11011101,11011011,11011111,1111100,1111010,1111110,1111001,1111101,1111011,1111111,11111100,11111010,11111110,11111001,11111101,11111011,11111111,1000101,1001001,1001101,1010001,1010101,1011001,1011101,11000101,11001001,11001101,11010001,11010101,11011001,11011101,1000111,1001011,1001111,1010011,1010111,1011011,1011111,11000111,11001011,11001111,11010011,11010111,11011011,11011111,1100101,1101001,1101101,1110001,1110101,1111001,1111101,11100101,11101001,11101101,11110001,11110101,11111001,11111101,1100111,1101011,1101111,1110011,1110111,1111011,1111111,11100111,11101011,11101111,11110011,11110111,11111011,11111111,1000101,100101,1100101,10101,1010101,110101,1110101,11000101,10100101,11100101,10010101,11010101,10110101,11110101,1000111,100111,1100111,10111,1010111,110111,1110111,11000111,10100111,11100111,10010111,11010111,10110111,11110111,1001101,101101,1101101,11101,1011101,111101,1111101,11001101,10101101,11101101,10011101,11011101,10111101,11111101,1001111,101111,1101111,11111,1011111,111111,1111111,11001111,10101111,11101111,10011111,11011111,10111111,11111111,1001010,1001011,1011010,1011011,110001,1110001,110101,1110101,10100100,10100101,10110100,10110101,10010010,11010010,10010110,11010110};


		
	
	
	
	int Imagedata_N[Row][Column][1] ;
	int Imagedata_M[Row][Column][1] ;
	unsigned char Imagedata_output[Row][Column][1];
	
	for(int i = 0; i < Row; i++){
		for(int j = 0; j < Column; j++){
			Imagedata_N[i][j][0] = (int)Imagedata[i][j][0] / 255;

		}
	}
	
	int _flag = 1;
	int iter = 0;
	
	while(_flag==1){
		_flag = 0;
		iter = iter + 1;
//		cout<<"iter times: "<< iter << endl;
		
		for(int i = 1; i < Row - 1; i++){
			for(int j = 1; j < Column - 1; j++){
				Imagedata_M[i][j][0] = 0;
				long int compare = 10000000*Imagedata_N[i][j+1][0] + 1000000*Imagedata_N[i-1][j+1][0] + 100000*Imagedata_N[i-1][j][0] + 10000*Imagedata_N[i-1][j-1][0] + 1000*Imagedata_N[i][j-1][0] + 100*Imagedata_N[i+1][j-1][0] + 10*Imagedata_N[i+1][j][0] + Imagedata_N[i+1][j+1][0];
				for(int k = 0; k < 40; k++){
					Imagedata_M[i][j][0] = Imagedata_N[i][j][0] && !(compare^conditional_mask[k]);
					if(Imagedata_M[i][j][0]){
						break;
					}
				}
			}
		}
	
		int k;
		for(int i = 1; i < Row - 1; i++){
			for(int j = 1; j < Column - 1; j++){
				int compare = 10000000*Imagedata_M[i][j+1][0] + 1000000*Imagedata_M[i-1][j+1][0] + 100000*Imagedata_M[i-1][j][0] + 10000*Imagedata_M[i-1][j-1][0] + 1000*Imagedata_M[i][j-1][0] + 100*Imagedata_M[i+1][j-1][0] + 10*Imagedata_M[i+1][j][0] + Imagedata_M[i+1][j+1][0];
				for(k = 0; k < 444; k++){
					if(!(compare^unconditional_mask[k])){
						break;
					}
				}
				if(k == 444){
					int temp = Imagedata_N[i][j][0];
					Imagedata_N[i][j][0] = Imagedata_N[i][j][0] && (!Imagedata_M[i][j][0]);
					if (temp != Imagedata_N[i][j][0]){
						_flag = 1;
					}
					
				}
			}
		}
	}
	cout<<"iter times: "<< iter << endl;

	//	briging
	for( int i = 0; i < Row; i++){
		for (int j = 0; j < Column; j++){
			
			int X  = Imagedata_N[i][j][0];
			int X0 = Imagedata_N[i][j+1][0];
			int X1 = Imagedata_N[i-1][j+1][0];
			int X2 = Imagedata_N[i-1][j][0];
			int X3 = Imagedata_N[i-1][j-1][0];
			int X4 = Imagedata_N[i][j-1][0];
			int X5 = Imagedata_N[i+1][j-1][0];
			int X6 = Imagedata_N[i+1][j][0];
			int X7 = Imagedata_N[i+1][j+1][0];
			
			
			int L1 = !X && !X0 &&  X1 && !X2 &&  X3 && !X4 && !X5 && !X6 && !X7;
			int L2 = !X && !X0 && !X1 && !X2 &&  X3 && !X4 &&  X5 && !X6 && !X7;
			int L3 = !X && !X0 && !X1 && !X2 && !X3 && !X4 &&  X5 && !X6 &&  X7;
			int L4 = !X && !X0 &&  X1 && !X2 && !X3 && !X4 && !X5 && !X6 &&  X7;
			
			int PQ = L1 || L2 || L3 || L4;
			
			int P1 = !X2 && !X6 && (X3 || X4 || X5) && (X0 || X1 || X7) && !PQ;
			int P2 = !X0 && !X4 && (X1 || X2 || X3) && (X5 || X6 || X7) && !PQ;
			int P3 = !X0 && !X6 && X7 && (X2 || X3 || X4);
			int P4 = !X0 && !X2 && X1 && (X4 || X5 || X6);
			int P5 = !X2 && !X4 && X3 && (X0 || X6 || X7);
			int P6 = !X4 && !X6 && X5 && (X0 || X1 || X2);
			
			Imagedata_N[i][j][0] = (X || P1 || P2 || P3 || P4 || P5 || P6);

		}
	}

	
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
	fwrite(Imagedata_output, sizeof(unsigned char), Row*Column*BytesPerPixel, file);
	fclose(file);

	return 0;
}
