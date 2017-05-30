//USC EE569 Homework-1 Problem-2-(c) by Xuejing Lei
//transform the histogram of an input image to normal distribution
//Email: xjlei1117@gmail.com
//Date: February 5, 2017

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fstream>

using namespace std;

#define PI 3.14159265

int get_gaussian(int gaus[], int SizeV, int SizeR)
{
	double *gausd = new double[256];
	double *gausd_new = new double[256];
	double delta = 20.0, mu = 70.0, sumd = 0.0;
	int sum = 0, dev = 0, flag = 0;

	for (int i=0; i<256; i++) {
		gausd[i] = exp(-1.0*(i-mu)*(i-mu)/2.0/delta/delta)/sqrt(2.0*PI*delta*delta);
		sumd = sumd + gausd[i];
	}
	for(int i=0; i<256; i++) {
		gausd_new[i] = gausd[i]/sumd;
		gaus[i] = round(SizeV*SizeR*gausd_new[i]);
		sum = sum+gaus[i];
		if (gaus[i] != 0) flag = i;	
	}
	if (sum < SizeV*SizeR) {
		dev = SizeV*SizeR-sum;
		for (int j=flag+1; j<flag+1+dev; j++) {
			gaus[j] = 1;
			sum = sum + gaus[j];
		}
	}

	if (sum != SizeV*SizeR) {
		cout<<"Wrong in get_gaussian!"<<endl;
		exit(1);
	}
	delete [] gausd;
	delete [] gausd_new;

	return 0;
}

int get_map(int occu[], int gaus[], int map[256][256], int start[], int end[])  // map[i][j]  # of pixels with original value i change to value j ; num[i] # of pieces devided of element i
{
	int i = 0, j = 0;
	memset(start, 0, 256*sizeof(int));
	memset(end, 0, 256*sizeof(int));
	start[0]=0;
	i = 0; j = 0;
	while (i<256 && j<256) {  // i trace position of occu[], j foor gaus[]
		if (occu[i] < gaus[j]) {
			map[i][j] = occu[i];
			gaus[j]=gaus[j]-map[i][j]; 
			i++; 
			start[i]=j; 
			end[i]=j;
		} else if (occu[i] > gaus[j]) {
			map[i][j] = gaus[j]; 
			occu[i]=occu[i]-map[i][j]; 
			j++; 
			end[i]=j;
		} else if (occu[i] == gaus[j]) {
			map[i][j] = occu[i]; 
			end[i]=j;
			i++; 
			j++; 
			start[i]=j; 
			end[i]=j;
		}
	}
	return 0;
}

int gaussian_trans_color(char *fn_pre, int SizeV, int SizeR)
{
	FILE *file, *file1;
	int BytesPerPixel = 3;
	char *filename = new char[30];
	int *gaus = new int[256];
	int *occu_r = new int[256];
	int *occu_b = new int[256];
	int *occu_g = new int[256];
	int *occu_r_n = new int[256];
	int *occu_b_n = new int[256];
	int *occu_g_n = new int[256];
	int map_r[256][256], map_g[256][256], map_b[256][256];
	int start_r[257], end_r[257], start_g[257], end_g[257], start_b[257], end_b[257];
	int sum = 0, sum1 = 0;
	
	
	memset(map_r, 0, 256*256*sizeof(int));
	memset(map_g, 0, 256*256*sizeof(int));
	memset(map_b, 0, 256*256*sizeof(int));
	memset(occu_r, 0, 256*sizeof(int));
	memset(occu_g, 0, 256*sizeof(int));
	memset(occu_b, 0, 256*sizeof(int));
	
	memset(start_r, 0, 257*sizeof(int));
	memset(start_g, 0, 257*sizeof(int));
	memset(start_b, 0, 257*sizeof(int));
	memset(end_r, 0, 257*sizeof(int));
	memset(end_g, 0, 257*sizeof(int));
	memset(end_b, 0, 257*sizeof(int));
	memset(occu_r_n, 0, 256*sizeof(int));
	memset(occu_g_n, 0, 256*sizeof(int));
	memset(occu_b_n, 0, 256*sizeof(int));

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}

	unsigned char ***ImageData = new unsigned char**[SizeR];
	unsigned char ***ImageData_New = new unsigned char**[SizeR];

	for (int i=0; i<SizeR; i++) {

		ImageData[i] = new unsigned char*[SizeV];
		ImageData_New[i] = new unsigned char*[SizeV];
		for (int j=0; j<SizeV; j++) {
			ImageData[i][j] = new unsigned char[BytesPerPixel];
			ImageData_New[i][j] = new unsigned char[BytesPerPixel];
			fread(ImageData[i][j], sizeof(unsigned char), BytesPerPixel, file);
			occu_r[(int)ImageData[i][j][0]]++;
			occu_g[(int)ImageData[i][j][1]]++;
			occu_b[(int)ImageData[i][j][2]]++;
		}
	}
	fclose(file);

	memset(gaus, 0, 256*sizeof(int));
	get_gaussian(gaus, SizeV, SizeR);

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_record.txt");
	ofstream fout(filename);
	fout<<"Gaussian function in "<<fn_pre<<".raw:"<<"\n";
	for (int i=0; i<256; i++) fout<<gaus[i]<<"\n";
	fout<<"Occurance of RED channel in "<<fn_pre<<".raw:"<<"\n";
	for (int i=0; i<256; i++) fout<<occu_r[i]<<"\n";
	fout<<"Occurance of GREEN channel in "<<fn_pre<<".raw:"<<"\n";
	for (int i=0; i<256; i++) fout<<occu_g[i]<<"\n";
	fout<<"Occurance of BLUE channel in "<<fn_pre<<".raw:"<<"\n";
	for (int i=0; i<256; i++) fout<<occu_b[i]<<"\n";


	get_map(occu_r, gaus, map_r, start_r, end_r);
	memset(gaus, 0, 256*sizeof(int));
	get_gaussian(gaus, SizeV, SizeR);
	get_map(occu_g, gaus, map_g, start_g, end_g);
	memset(gaus, 0, 256*sizeof(int));
	get_gaussian(gaus, SizeV, SizeR);
	get_map(occu_b, gaus, map_b, start_b, end_b);

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_Gaussian.raw");
	if (!(file1=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	for(int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			for (int k=start_r[(int)ImageData[i][j][0]]; k<=end_r[(int)ImageData[i][j][0]]; k++) {
				if (map_r[(int)ImageData[i][j][0]][k] != 0) {
					ImageData_New[i][j][0] = k;
					map_r[(int)ImageData[i][j][0]][k]--;
					break;
				}
			}
			for (int k=start_g[(int)ImageData[i][j][1]]; k<=end_g[(int)ImageData[i][j][1]]; k++) {
				if (map_g[(int)ImageData[i][j][1]][k] != 0) {
					ImageData_New[i][j][1] = k;
					map_g[(int)ImageData[i][j][1]][k]--;
					break;
				}
			}
			for (int k=start_b[(int)ImageData[i][j][2]]; k<=end_b[(int)ImageData[i][j][2]]; k++) {
				if (map_b[(int)ImageData[i][j][2]][k] != 0) {
					ImageData_New[i][j][2] = k;
					map_b[(int)ImageData[i][j][2]][k]--;
					break;
				}
			}
			occu_r_n[(int)ImageData_New[i][j][0]]++;
			occu_g_n[(int)ImageData_New[i][j][1]]++;
			occu_b_n[(int)ImageData_New[i][j][2]]++;
			fwrite(ImageData_New[i][j], sizeof(unsigned char), BytesPerPixel, file1);
		}
	}
	fclose(file1);

	fout<<"New occurance of RED channel in "<<fn_pre<<".raw:"<<"\n";
	for (int i=0; i<256; i++) fout<<occu_r_n[i]<<"\n";
	fout<<"New occurance of GREEN channel in "<<fn_pre<<".raw:"<<"\n";
	for (int i=0; i<256; i++) fout<<occu_g_n[i]<<"\n";
	fout<<"new occurance of BLUE channel in "<<fn_pre<<".raw:"<<"\n";
	for (int i=0; i<256; i++) fout<<occu_b_n[i]<<"\n";
	fout.close();

	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			delete [] ImageData[i][j];
			delete [] ImageData_New[i][j];
		}
	}
	for (int i=0; i<SizeR; i++) {
		delete [] ImageData[i];
		delete [] ImageData_New[i];
	}
	delete [] ImageData;
	delete [] ImageData_New;
	delete [] gaus;
	delete [] occu_r;
	delete [] occu_g;
	delete [] occu_b;
	delete [] occu_r_n;
	delete [] occu_g_n;
	delete [] occu_b_n;
	delete [] filename;
}

int main(int argc, char *argv[])
{
	int BytesPerPixel = 3; //gray image
	int SizeV_f1 = 960, SizeR_f1 = 600; // size of image forest_1.raw
	int SizeV_f2 = 550, SizeR_f2 = 413; // size of image forest_1.raw


	// Check for proper syntax
	if (argc < 3 || (argc > 3 && argc < 7)){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name forest_1 forest_2 [VolumeSize1 = 960] [RowSize1 = 600] [VolumeSize2 = 550] [RowSize2 = 413]" << endl;
		return 0;
	} else if (argc > 3) {
		SizeV_f1 = atoi(argv[3]);
		SizeR_f1 = atoi(argv[4]);
		SizeV_f2 = atoi(argv[5]);
		SizeR_f2 = atoi(argv[6]);
	}

	gaussian_trans_color(argv[1], SizeV_f1, SizeR_f1);
	gaussian_trans_color(argv[2], SizeV_f2, SizeR_f2);

	return 0;
}