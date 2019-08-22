			//(1) Name: Yifei Liu
			//(2) USC ID Number: 3852294243 
			//(3) USC Email: liu534@usc.edu
			//(4) Submission Date: 1/22/2019

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;
char MBVQ[5];
char vertex[10];

int determined_MBVQ(int R, int G, int B){
	if ((R+G) > 255){
		if((G+B) > 255){
			if((R+G+B) > 510){
				strcpy(MBVQ, "CMYW");
			}
			else{
				strcpy(MBVQ, "MYGC");
			}
		}
		else{
			strcpy(MBVQ, "RGMY");
		}
	}
	else{
		if(!((G+B) > 255)){
			if(!((R+G+B) > 255)){
				strcpy(MBVQ,"KRGB");
			}
			else{
				strcpy(MBVQ,"RGBM");
			}
		}
		else{
			strcpy(MBVQ,"CMGB");
		}
	}
	
	return 1;
	
}

int determined_vertex(double R, double G, double B){
	if (strcmp(MBVQ, "CMYW") == 0){
		strcpy(vertex,"white");
		if (B < 0.5 && B <= R && B <= G){
			strcpy(vertex,"yellow");
		}
		else if(G < 0.5 && G <= B && G <= R){
			strcpy(vertex,"magenta");
		}
		else if(R < 0.5 && R <= B && R <= G){
			strcpy(vertex,"cyan");
		}
	}
	
	else if(strcmp(MBVQ, "MYGC") == 0){
		strcpy(vertex,"magenta");
		if (R >= B && G >= B){
			if(R >= 0.5){
				strcpy(vertex,"yellow");
			}
			else{
				strcpy(vertex,"green");
			}
		}
		if(G >= R && B >= R ){
			if(B >= 0.5){
				strcpy(vertex,"cyan");
			}
			else{
				strcpy(vertex,"green");
			}
		}
	}
	
	else if(strcmp(MBVQ, "RGMY") == 0){
		if(B > 0.5){
			if(R > 0.5){
				if (B >= G){
					strcpy(vertex,"magenta");
				}
				else{
					strcpy(vertex,"yellow");
				}
			}
			else{
				if(G > (B + R)){
					strcpy(vertex,"green");
				}
				else{
					strcpy(vertex,"magenta");
				}
			}
		}
		else{
			if (R>=0.5){
				if (G >= 0.5){
					strcpy(vertex,"yellow");
				}
				else{
					strcpy(vertex,"red");
				}
			}
			else{
				if( R >= G){
					strcpy(vertex,"red");
				}
				else{
					strcpy(vertex,"green");
				}
			}
		}

	}
	
	else if(strcmp(MBVQ, "KRGB") == 0){
		strcpy(vertex,"black");
		if (B > 0.5 && B >= R && B >= G){
			strcpy(vertex,"blue");
		}
		else if(G > 0.5 && G >= B && G >= R){
			strcpy(vertex,"green");
		}
		else if(R > 0.5 && R >= B && R >= G){
			strcpy(vertex,"red");
		}
	}
	else if(strcmp(MBVQ, "RGBM") == 0){
		strcpy(vertex,"green");
		if (R > G && R >= B){
			if(B < 0.5){
				strcpy(vertex,"red");
			}
			else{
				strcpy(vertex,"magenta");
			}
		}
		if(B > G && B >= R ){
			if(R < 0.5){
				strcpy(vertex,"blue");
			}
			else{
				strcpy(vertex,"magenta");
			}
		}
	
	}
	else if(strcmp(MBVQ, "CMGB") == 0){
		if(B > 0.5){
			if(R > 0.5){
				if (G >= R){
					strcpy(vertex,"cyan");
				}
				else{
					strcpy(vertex,"magenta");
				}
			}
			else{
				if(G > 0.5){
					strcpy(vertex,"cyan");
				}
				else{
					strcpy(vertex,"blue");
				}
			}
		}
		else{
			if (R > 0.5){
				if ((R - G + B) >= 0.5){
					strcpy(vertex,"magenta");
				}
				else{
					strcpy(vertex,"green");
				}
			}
			else{
				if( G >= B){
					strcpy(vertex,"green");
				}
				else{
					strcpy(vertex,"blue");
				}
			}
		}

	}
					
	
		
	return 1;

}

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
	

	int R,G,B;
	double C,M,Y;
	int i,j,k,m,n;
	
	
	
	//1. Expand the image matrix.----------------------------------------------------------
	
	cout << "1.Expend the matrix" <<endl;


	int N = 5; //Window size



	double Imagedata_Expassion[Row + N - 1][Column + N - 1][BytesPerPixel];

	//fill inside

	for (k = 0; k < BytesPerPixel; k++){
		for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
			for (j = (N - 1)/2; j < Column + (N - 1)/2; j++){

				Imagedata_Expassion[i][j][k] = Imagedata[i - (N - 1)/2][j - (N - 1)/2][k];
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
	
	double Jarvis[5][5] = {{0.0,0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0,0.0},{0.0,0.0,0.0,7.0,5.0},{3.0,5.0,7.0,5.0,3.0},{1.0,3.0,5.0,3.0,1.0}};
	double Jarvis_Mirror[5][5] = {{0.0},{0.0},{5.0,7.0,0.0,0.0,0.0},{3.0,5.0,7.0,5.0,3.0},{1.0,3.0,5.0,3.0,1.0}};
	
	
	//scanning------------------------------------------------------------------------
	unsigned char Imagedata_Diffusion[Row][Column][BytesPerPixel];
	double err[3] = {0.0};
	int flag = 0;
	double R_new,G_new,B_new;
	for (i = (N - 1)/2; i < Row + (N - 1)/2; i++){
		if (i % 2 == 0){
			for(j = (N - 1)/2; j < Column + (N - 1)/2; j++){

				R = Imagedata[i - (N - 1)/2][j - (N - 1)/2][0];
				G = Imagedata[i - (N - 1)/2][j - (N - 1)/2][1];
				B = Imagedata[i - (N - 1)/2][j - (N - 1)/2][2];
						
				flag = determined_MBVQ(R, G, B);
				if (flag != 1){ cout << " determined MBVQ error! " <<endl; }
			
				R_new = Imagedata_Expassion[i][j][0] / 255.0;
				G_new = Imagedata_Expassion[i][j][1] / 255.0;
				B_new = Imagedata_Expassion[i][j][2] / 255.0;
			
				flag = 0;
				flag = determined_vertex(R_new, G_new ,B_new);
//				cout<<vertex<<endl;
					
				if (flag != 1){ cout << " determined MBVQ error! " <<endl; }
			
				if (strcmp(vertex,"white") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 255;
				}
				else if (strcmp(vertex,"black") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 0;
				}
				else if (strcmp(vertex,"red") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 0;
				}
				else if (strcmp(vertex,"green") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 0;
				}
				else if (strcmp(vertex,"blue") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 255;
				}
				else if (strcmp(vertex,"magenta") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 255;
				}
				else if (strcmp(vertex,"cyan") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 255;
				}
				else if (strcmp(vertex,"yellow") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 0;
				}
	
				for(k = 0; k < BytesPerPixel; k++){
					err[k] = Imagedata_Expassion[i][j][k] - Imagedata_Diffusion[i - (N - 1)/2][j - (N - 1)/2][k];
					for (m = 0; m < N; m++){
						for(n = 0;n < N;n++){
							Imagedata_Expassion[i - (N - 1)/2 + m][j - (N - 1)/2 + n][k] += err[k] * Jarvis[m][n] / 48.0;
						}
					}
				}				
				}
			}
		else if(i % 2 == 1){
			for(j = Column + (N - 1)/2 - 1; j >= (N - 1)/2; j--){

				R = Imagedata[i - (N - 1)/2][j - (N - 1)/2][0];
				G = Imagedata[i - (N - 1)/2][j - (N - 1)/2][1];
				B = Imagedata[i - (N - 1)/2][j - (N - 1)/2][2];
						
				flag = determined_MBVQ(R, G, B);
				if (flag != 1){ cout << " determined MBVQ error! " <<endl; }
			
				R_new = Imagedata_Expassion[i][j][0] / 255.0;
				G_new = Imagedata_Expassion[i][j][1] / 255.0;
				B_new = Imagedata_Expassion[i][j][2] / 255.0;
			
				flag = 0;
				flag = determined_vertex(R_new, G_new ,B_new);
//				cout<<vertex<<endl;
					
				if (flag != 1){ cout << " determined MBVQ error! " <<endl; }
			
				if (strcmp(vertex,"white") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 255;
				}
				else if (strcmp(vertex,"black") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 0;
				}
				else if (strcmp(vertex,"red") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 0;
				}
				else if (strcmp(vertex,"green") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 0;
				}
				else if (strcmp(vertex,"blue") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 255;
				}
				else if (strcmp(vertex,"magenta") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 255;
				}
				else if (strcmp(vertex,"cyan") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 0;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 255;
				}
				else if (strcmp(vertex,"yellow") == 0){
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][0] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][1] = 255;
					Imagedata_Diffusion[i-(N - 1)/2][j-(N - 1)/2][2] = 0;
				}
	
				for(k = 0; k < BytesPerPixel; k++){
					err[k] = Imagedata_Expassion[i][j][k] - Imagedata_Diffusion[i - (N - 1)/2][j - (N - 1)/2][k];
					for (m = 0; m < N; m++){
						for(n = 0;n < N;n++){
							Imagedata_Expassion[i - (N - 1)/2 + m][j - (N - 1)/2 + n][k] += err[k] * Jarvis_Mirror[m][n] / 48.0;
						}
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
	fwrite(Imagedata_Diffusion, sizeof(unsigned char), Row*Column*BytesPerPixel, file);
	fclose(file);

	return 0;
}
