//USC EE569 Homework-1 Problem-2-(a) by Xuejing Lei
//Two methods of histogram enhancement for grayscale images
//Method A: the linear transfer-function-based histogram enhancement method
//Method B: the cumulative-probability-based histogram equalization method
//Email: xjlei1117@gmail.com
//Date: February 5, 2017

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fstream>

using namespace std;

int max(int a, int b)
{
	int result = 0;
	if (a<b) result = b;
	else result = a;
	return result;
}

int min(int a, int b)
{
	int result = 0;
	if (a<b) result = a;
	else result = b;
	return result;
}

int linear_trans_func(int Data_Old, int min_o, int max_o, int min_n, int max_n)
{
	float k = 0.0, res = 0.0;

	k = ((float)(max_n - min_n))/((float)(max_o - min_o));
	res = k*(Data_Old - min_o) + min_n;

	return round(res);
}

int linear_trans_func_gray(char *fn_pre, int SizeV, int SizeR)
{
	FILE *file, *file1;
	char *filename = new char[30];
	int min_f = 255, max_f = 0;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}

	unsigned char **GrayData = new unsigned char*[SizeR];
	unsigned char **GrayData_New = new unsigned char*[SizeR];
	int *occurance = new int[256];
	memset(occurance, 0, 256*sizeof(int));

	for (int i=0; i<SizeR; i++) {
		GrayData[i] = new unsigned char[SizeV];
		GrayData_New[i] = new unsigned char[SizeV];
		fread(GrayData[i], sizeof(unsigned char), SizeV, file);
		for (int j=0; j<SizeV; j++) {
			if ((int)GrayData[i][j]>=0 && (int)GrayData[i][j]<256) {
				occurance[((int)GrayData[i][j])]++;
				// cout << "occurance["<<(int)GrayData[i][j]<<"] = "<<occurance[(int)GrayData[i][j]]<<" ";
				max_f = max(max_f, GrayData[i][j]);
				min_f = min(min_f, GrayData[i][j]);
			}
			else {
				cout << "Data is wrong at row "<<i<<" volume "<<j<<endl;
				exit(1);
			}
		}
	}
	fclose(file);

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_occurance_A.txt");
	ofstream fout(filename);
	for (int i=0; i<256; i++) {
		fout<<occurance[i]<<"\n";
	}
	fout.close();
	
	cout<<"Range of values in original image "<<fn_pre<<".raw is ["<<min_f<<", "<<max_f<<"]."<<endl;
	
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_MethodA.raw");
	if (!(file1=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			GrayData_New[i][j] = linear_trans_func(GrayData[i][j], min_f, max_f, 0, 255);
		}
		fwrite(GrayData_New[i], sizeof(unsigned char), SizeV, file1);
	}
	fclose(file1);

	for (int i=0; i<SizeR; i++) {
		delete [] GrayData[i];
		delete [] GrayData_New[i];
	}
	delete [] GrayData;
	delete [] GrayData_New;
	delete [] filename;
	delete [] occurance;

	return 0;
}

int revised_linear_trans_func_gray(char *fn_pre, int SizeV, int SizeR)
{
	FILE *file, *file1;
	char *filename = new char[30];
	int min_f1 = 255, max_f1 = 0, min_f2 = 255, max_f2 = 0;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}

	unsigned char **GrayData = new unsigned char*[SizeR];
	unsigned char **GrayData_New = new unsigned char*[SizeR];
	int *occurance = new int[256];
	memset(occurance, 0, 256*sizeof(int));

	for (int i=0; i<SizeR; i++) {
		GrayData[i] = new unsigned char[SizeV];
		GrayData_New[i] = new unsigned char[SizeV];
		fread(GrayData[i], sizeof(unsigned char), SizeV, file);
		for (int j=0; j<SizeV; j++) {
			if ((int)GrayData[i][j]>=0 && (int)GrayData[i][j]<256) {
				occurance[((int)GrayData[i][j])]++;
				// cout << "occurance["<<(int)GrayData[i][j]<<"] = "<<occurance[(int)GrayData[i][j]]<<" ";	
			}
			else {
				cout << "Data is wrong at row "<<i<<" volume "<<j<<endl;
				exit(1);
			}
		}
	}
	fclose(file);

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_occurance_A_Rev.txt");
	ofstream fout(filename);

	for (int i=0; i<256; i++) {
		fout<<occurance[i]<<"\n";
		if (min_f1 == 255 && occurance[i] != 0) {
			min_f1 = i;
			max_f1 = min_f1;
		} 
		if (i<254) {
			if(max_f1 >= min_f1 && occurance[i] != 0 && occurance[i+1] == 0 && occurance [i+2] == 0) {
				max_f1 = i; 
				min_f2 = max_f1;
				max_f2 = min_f2;
			} 
		}
		if (i > 1) {
			if (min_f2 >= max_f1 && occurance[i] != 0 && occurance[i-2] == 0 && occurance [i-1] == 0) {
				min_f2 = i;
				max_f2 = min_f2;
			}
			if (max_f2 >= min_f2) max_f2 = i;
		} 
	}
	fout.close();

	cout<<"First range of values in original image "<<fn_pre<<".raw is ["<<min_f1<<", "<<max_f1<<"]."<<endl;
	cout<<"Second range of values in original image "<<fn_pre<<".raw is ["<<min_f2<<", "<<max_f2<<"]."<<endl;
	
	int split = 0, total = 0;
	total = (max_f1 - min_f1) + (max_f2 - min_f2);
	split = floor(255.0/total*(max_f1 - min_f1));
	cout<<"split: "<<split<<endl;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_MethodA_Revised.raw");
	if (!(file1=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			if (GrayData[i][j] >= min_f1 && GrayData[i][j] <= max_f1) {
				GrayData_New[i][j] = linear_trans_func(GrayData[i][j], min_f1, max_f1, 0, split);
			} else if (GrayData[i][j] >= min_f2 && GrayData[i][j] <= max_f2) {
				GrayData_New[i][j] = linear_trans_func(GrayData[i][j], min_f2, max_f2, split+1, 255);
			}
		}
		fwrite(GrayData_New[i], sizeof(unsigned char), SizeV, file1);
	}
	fclose(file1);

	for (int i=0; i<SizeR; i++) {
		delete [] GrayData[i];
		delete [] GrayData_New[i];
	}
	delete [] GrayData;
	delete [] GrayData_New;
	delete [] filename;
	delete [] occurance;

	return 0;
}

int cdf_trans_func(int cdf, int min_cdf, int max_cdf)
{
	float k = 0.0;
	k = 255.0/(max_cdf - min_cdf);
	return round(k*(cdf - min_cdf));
}

int cdf_trans_gray(char *fn_pre, int SizeV, int SizeR)
{
	FILE *file, *file1;
	char *filename = new char[30];
	int min_f = 255, max_f = 0;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	} 

	unsigned char **GrayData = new unsigned char*[SizeR];
	unsigned char **GrayData_New = new unsigned char*[SizeR];
	int *occurance = new int[256];
	int *cdf = new int[256];
	memset(occurance, 0, 256*sizeof(int));
	memset(cdf, 0, 256*sizeof(int));

	for (int i=0; i<SizeR; i++) {
		GrayData[i] = new unsigned char[SizeV];
		GrayData_New[i] = new unsigned char[SizeV];
		fread(GrayData[i], sizeof(unsigned char), SizeV, file);
		for (int j=0; j<SizeV; j++) {
			if ((int)GrayData[i][j]>=0 && (int)GrayData[i][j]<256) {
				occurance[((int)GrayData[i][j])]++;
				max_f = max(max_f, GrayData[i][j]);
				min_f = min(min_f, GrayData[i][j]);
			}
			else {
				cout << "Data is wrong at row "<<i<<" volume "<<j<<endl;
				exit(1);
			}
		}
	}
	fclose(file);

	int sum = 0;
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_occurance_B.txt");
	ofstream fout(filename);
	fout<<"Occurance:"<<"\n";
	for (int i=0; i<256; i++) {
		if (i >= min_f && i <= max_f) {
			if (occurance[i] != 0) {
				if (i == min_f) cdf[i] = occurance[i];
				else cdf[i] = sum +occurance[i];
				sum = cdf[i];
			} else {
				cdf[i] = 0;
			}
		} else {
			cdf[i] = 0;
		}
		fout<<occurance[i]<<"\n";
	}
	fout<<"\n\nCDF:"<<"\n";
	for (int i=0; i<256; i++) fout<<cdf[i]<<"\n";
	fout<<"\n\nNew_Intensity:"<<"\n";
	for (int i=0; i<256; i++) fout<<cdf_trans_func(cdf[i], cdf[min_f], cdf[max_f])<<"\n";
	fout.close();
	
	cout<<"Range of values in original image "<<fn_pre<<".raw is ["<<min_f<<", "<<max_f<<"]."<<endl;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_MethodB.raw");
	if (!(file1=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			GrayData_New[i][j] = cdf_trans_func(cdf[(int)GrayData[i][j]], cdf[min_f], cdf[max_f]);
		}
		fwrite(GrayData_New[i], sizeof(unsigned char), SizeV, file1);
	}
	fclose(file1);

	for (int i=0; i<SizeR; i++) {
		delete [] GrayData[i];
		delete [] GrayData_New[i];
	}
	delete [] GrayData;
	delete [] GrayData_New;
	delete [] filename;
	delete [] occurance;
	delete [] cdf;

	return 0;
}

int main(int argc, char *argv[])
{
	int BytesPerPixel = 1; //gray image
	int SizeV = 400, SizeR = 366; // size of image

	// Check for proper syntax
	if (argc < 4 ){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name tulip_dark tulip_bright tulip_mix [VolumeSize = 400] [RowSize = 366]" << endl;
		return 0;
	} else if (argc > 4) {
		SizeV = atoi(argv[4]);
		SizeR = atoi(argv[5]);
	}
	
	linear_trans_func_gray(argv[1], SizeV, SizeR);
	linear_trans_func_gray(argv[2], SizeV, SizeR);
	linear_trans_func_gray(argv[3], SizeV, SizeR);

	cdf_trans_gray(argv[1], SizeV, SizeR);
	cdf_trans_gray(argv[2], SizeV, SizeR);
	cdf_trans_gray(argv[3], SizeV, SizeR); 

	// revised linear transfer function based method A for tulip_mix 
	// which divides data into two parts
	revised_linear_trans_func_grsay(argv[3], SizeV, SizeR);

	return 0;
}