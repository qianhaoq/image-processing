//USC EE569 Homework-1 Problem-1-(a) by Xuejing Lei
//Basic mirroring, resizing, compositing
//Email: xjlei1117@gmail.com
//Date: February 5, 2017

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>

using namespace std;

int mirroring(char *fn_pre, int SizeV, int SizeR, int BytesPerPixel)
{
	FILE *file, *file1;
	char *filename = new char[30];
    
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_Mirroring.raw");
	if (!(file1=fopen(filename,"wb"))) {
	cout << "Cannot open file: " << filename << endl;
		exit(1);
	}

	unsigned char ***ImageData = new unsigned char **[SizeR];
	unsigned char ***Mirroring = new unsigned char **[SizeR];

	for (int i=0; i<SizeR; i++){
		ImageData[i] = new unsigned char *[SizeV];
		Mirroring[i] = new unsigned char *[SizeV];
	 	for (int j=0; j<SizeV; j++){
	 		ImageData[i][j] = new unsigned char[BytesPerPixel];
	 		Mirroring[i][j] = new unsigned char[BytesPerPixel];
	 		fread(ImageData[i][j], sizeof(unsigned char), BytesPerPixel, file);
	 	}
	 }	
	fclose(file);

	for (int i=0; i<SizeR; i++){
	 	for (int j=0; j<SizeV; j++){
	 		for (int k=0; k<BytesPerPixel; k++){
	 			Mirroring[i][j][k] = ImageData[i][SizeV-j-1][k];
	 		}
	 		fwrite(Mirroring[i][j], sizeof(unsigned char), BytesPerPixel, file1);
	 	}
	 }
	 fclose(file1);	

	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			delete [] ImageData[i][j];
			delete [] Mirroring[i][j];
		}
	}
	for (int i=0; i<SizeR; i++) {
		delete [] ImageData[i];
		delete [] Mirroring[i];
	}
	delete [] ImageData;
	delete [] Mirroring;
	delete [] filename;

	return 0;
}

int resizing(char *fn_pre, int SizeV, int SizeR, int SizeV_New, int SizeR_New, int BytesPerPixel)
{
	FILE *file, *file1;
	char *filename = new char[30];
	char *temp = new char[30];
	float dealtx2, dealty2;
	int xint2, yint2;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	unsigned char ***ImageData = new unsigned char **[SizeR];
	for (int i=0; i<SizeR; i++){
		ImageData[i] = new unsigned char *[SizeV];
	 	for (int j=0; j<SizeV; j++){
	 			ImageData[i][j] = new unsigned char[BytesPerPixel];
	 			fread(ImageData[i][j], sizeof(unsigned char), BytesPerPixel, file);
	 	}
	 }
	fclose(file);

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_Resizing_");
	sprintf(temp, "%d", SizeR_New);
	strcat(filename, temp);
	strcat(filename, ".raw");
	if (!(file1=fopen(filename,"wb"))) {
	cout << "Cannot open file: " << filename << endl;
		exit(1);
	}

	unsigned char ***Resizing = new unsigned char **[SizeR_New];

	for (int i=0; i<SizeR_New; i++){
		Resizing[i] = new unsigned char *[SizeV_New];
	 	for (int j=0; j<SizeV_New; j++){
	 		Resizing[i][j] = new unsigned char[BytesPerPixel];
	 		xint2 = floor(i*(float)SizeR/(float)SizeR_New); 
	 		yint2 = floor(j*(float)SizeV/(float)SizeV_New);
	 		dealtx2 = i*(float)SizeR/(float)SizeR_New - xint2; 
	 		dealty2 = j*(float)SizeV/(float)SizeV_New - yint2;
	 		for (int k=0; k<BytesPerPixel; k++){
	 			if ((i*(float)SizeR/(float)SizeR_New) < (SizeR - 1) && (j*(float)SizeV/(float)SizeV_New)< (SizeV - 1)) {
	 				Resizing[i][j][k] = (1-dealtx2)*(1-dealty2)*ImageData[xint2][yint2][k] + dealtx2*(1-dealty2)*ImageData[xint2][yint2+1][k]
	 					+ (1-dealtx2)*dealty2*ImageData[xint2+1][yint2][k] + dealtx2*dealty2*ImageData[xint2+1][yint2+1][k];
	 			} else  {
	 				Resizing[i][j][k] = ImageData[xint2][yint2][k];
	 			}
	 		}
	 		fwrite(Resizing[i][j], sizeof(unsigned char), BytesPerPixel, file1);
	 	}
	 }	
	fclose(file1);

	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			delete [] ImageData[i][j];
		}
	}
	for (int i=0; i<SizeR_New; i++) {
		for (int j=0; j<SizeV_New; j++) {
			delete [] Resizing[i][j];
		}
	}
	for (int i=0; i<SizeR; i++) {
		delete [] ImageData[i];
	}
	for (int i=0; i<SizeR_New; i++) {
		delete [] Resizing[i];
	}
	delete [] ImageData;
	delete [] Resizing;
	delete [] temp;
	delete [] filename;

	return 0;
}

int compositing(char *fn1_pre, char *fn2_pre, int SizeV_I1, int SizeR_I1, int SizeV_I2, int SizeR_I2, int BytesPerPixel)
{
	FILE *file, *file1, *file2;
	char *filename = new char[30];

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn1_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	unsigned char ***ImageData = new unsigned char **[SizeR_I1];
	for (int i=0; i<SizeR_I1; i++){
		ImageData[i] = new unsigned char *[SizeV_I1];
	 	for (int j=0; j<SizeV_I1; j++){
	 		ImageData[i][j] = new unsigned char[BytesPerPixel];
	 		fread(ImageData[i][j], sizeof(unsigned char), BytesPerPixel, file);
	 	}
	 }
	fclose(file);

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn2_pre);
	strcat(filename, ".raw");
	if (!(file1=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	unsigned char ***ImageData1 = new unsigned char **[SizeR_I2];
	for (int i=0; i<SizeR_I2; i++){
		ImageData1[i] = new unsigned char *[SizeV_I2];
	 	for (int j=0; j<SizeV_I2; j++){
	 		ImageData1[i][j] = new unsigned char[BytesPerPixel];
	 		fread(ImageData1[i][j], sizeof(unsigned char), BytesPerPixel, file1);
	 	}
	 }
	fclose(file1);

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn1_pre);
	strcat(filename, "ON");
	strcat(filename, fn2_pre);
	strcat(filename, ".raw");
	if (!(file2=fopen(filename,"wb"))) {
	cout << "Cannot open file: " << filename << endl;
		exit(1);
	}

	for (int i=0; i<SizeR_I1; i++) {
	 	for (int j=0; j<SizeV_I1; j++) {
	 		if ((ImageData[i][j][0] <100 && ImageData[i][j][1] < 100 && ImageData[i][j][2] > 140) ||  // remove dark blue
				(ImageData[i][j][0] <187 && ImageData[i][j][1] < 180 && ImageData[i][j][2] > 200)) {  //remove light blue
				continue;
			} else {
	 			ImageData1[i+400][j+1100][0] = ImageData[i][j][0];
	 			ImageData1[i+400][j+1100][1] = ImageData[i][j][1];
	 			ImageData1[i+400][j+1100][2] = ImageData[i][j][2];
			}
	 	}
	 }	
	for (int i=0; i<SizeR_I2; i++){
	 	for (int j=0; j<SizeV_I2; j++){
	 		fwrite(ImageData1[i][j], sizeof(unsigned char), BytesPerPixel, file2);
	 	}
	 }
	fclose(file2);

	for (int i=0; i<SizeR_I1; i++) {
		for (int j=0; j<SizeV_I1; j++) {
			delete [] ImageData[i][j];
		}
	}
	for (int i=0; i<SizeR_I2; i++) {
		for (int j=0; j<SizeV_I2; j++) {
			delete [] ImageData1[i][j];
		}
	}
	for (int i=0; i<SizeR_I1; i++) {
		delete [] ImageData[i];
	}
	for (int i=0; i<SizeR_I2; i++) {
		delete [] ImageData1[i];
	}
	delete [] ImageData;
	delete [] ImageData1;
	delete [] filename;

	return 0;
}

int main(int argc, char *argv[])
{
	FILE *file, *file1;
	int BytesPerPixel;
	int SizeV_Do = 300, SizeR_Do = 300;
	int SizeV_Be = 1914, SizeR_Be = 808;
	int SizeV_New = 0, SizeR_New = 0;
	
	// Check for proper syntax
	if (argc < 2){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name dog beach [VolumeSize1 = 300] [RowSize1 = 300] [VolumeSize2 = 1914] [RowSize2 = 808] [BytesPerPixel = 3]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color and its size
	if (argc < 4) {
		BytesPerPixel = 3;
	} else {
		SizeV_Do = atoi(argv[3]);
		SizeR_Do = atoi(argv[4]);
		if (argc >5) {
			SizeV_Be = atoi(argv[5]);
			SizeR_Be = atoi(argv[6]);
		}
		if (argc < 8) BytesPerPixel = 3;
		else BytesPerPixel = atoi(argv[7]); // default is color image, grey image is 1
	}

	//Dog Mirroring
	mirroring(argv[1], SizeV_Do, SizeR_Do, BytesPerPixel);

	// Dog Resizing
	// Resize to 200*200
	SizeV_New = 200;
	SizeR_New = 200;
	resizing(argv[1], SizeV_Do, SizeR_Do, SizeV_New, SizeR_New, BytesPerPixel);

	// Resize to 400*400
	SizeV_New = 400;
	SizeR_New = 400;
	resizing(argv[1], SizeV_Do, SizeR_Do, SizeV_New, SizeR_New, BytesPerPixel);

	// Resize to 600*600
	SizeV_New = 600;
	SizeR_New = 600;
	resizing(argv[1], SizeV_Do, SizeR_Do, SizeV_New, SizeR_New, BytesPerPixel);


	//Image Compositing
	char *filename = new char[30];

	memset(filename, 0, sizeof(filename));
	strcpy(filename, argv[1]);
	strcat(filename, "_Mirroring");
	compositing(filename, argv[2], SizeV_Do, SizeR_Do, SizeV_Be, SizeR_Be, BytesPerPixel);

	return 0;
}

