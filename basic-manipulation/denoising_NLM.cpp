//USC EE569 Homework-1 Problem-3-(b) by Xuejing Lei
//remove mixed noises using non-local means filter
//Email: xjlei1117@gmail.com
//Date: February 5, 2017

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fstream>

#define PI 3.14159265

using namespace std;

int read_image(char *fn_pre, int SizeV, int SizeR, int BytesPerPixel, unsigned char *ImageData)
{
	FILE *file;
	char *filename = new char[30];

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	fread(ImageData, sizeof(unsigned char), SizeR*SizeV*BytesPerPixel, file);
	fclose(file);

	delete [] filename;

	return 0;
}

int write_image(char *fn_pre, char *fn_out, int SizeV, int SizeR, int BytesPerPixel, unsigned char *ImageData)
{
	FILE *file;
	char *filename = new char[30];

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_");
	strcat(filename, fn_out);
	strcat(filename, ".raw");

	if (!(file=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	fwrite(ImageData, sizeof(unsigned char), SizeR*SizeV*BytesPerPixel, file);
	fclose(file);

	delete [] filename;

	return 0;
}

int cal_occu(char *fn_pre, int SizeV, int SizeR, int BytesPerPixel, unsigned char *ImageData, int occu[3][256])
{
	FILE *file;
	char *filename = new char[30];

	memset(occu, 0, 3*256*sizeof(int));
	for (int i=0; i<SizeR*SizeV*BytesPerPixel; i++) {
		if ((int)ImageData[i]>=0 && (int)ImageData[i]<256) {
			occu[i%BytesPerPixel][(int)ImageData[i]]++;
		} else {
			cout << "Data is wrong at row "<<i/BytesPerPixel/SizeV<<" volume "<<i/BytesPerPixel%SizeV<<endl;
 			exit(1);
		}
	}

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_occurance.txt");
	ofstream fout(filename);
	fout<<"Occurance of RED channel:"<<"\n";
	for (int i=0; i<256; i++) fout<<occu[0][i]<<"\n";

	fout<<"\n\nOccurance of GREEN channel:"<<"\n";
	for (int i=0; i<256; i++) fout<<occu[1][i]<<"\n";

	fout<<"\n\nOccurance of BLUE channel:"<<"\n";
	for (int i=0; i<256; i++) fout<<occu[2][i]<<"\n";
	fout.close();

	delete [] filename;

	return 0;
}

int cal_psnr(char *fn, unsigned char *ImageData, unsigned char *ImageData_ORI, int SizeV, int SizeR, int BytesPerPixel)
{
	int cha = 0;
	unsigned long long int MSE_R = 0, MSE_G = 0, MSE_B = 0;
	double psnr_r = 0.0, psnr_g = 0.0, psnr_b = 0.0;

	for (int i=0; i<SizeR*SizeV*BytesPerPixel; i++) {
		cha = i%3;
		if (cha == 0) {
			MSE_R = MSE_R + (ImageData[i]-ImageData_ORI[i])*(ImageData[i]-ImageData_ORI[i]);
		} else if (cha == 1) {
			MSE_G = MSE_G + (ImageData[i]-ImageData_ORI[i])*(ImageData[i]-ImageData_ORI[i]);
		} else if (cha == 2) {
			MSE_B = MSE_B + (ImageData[i]-ImageData_ORI[i])*(ImageData[i]-ImageData_ORI[i]);
		}		
	}

	if (MSE_R != 0) {
		psnr_r =  10*log10(255*255/(MSE_R/SizeR/SizeV));
		cout<<"PSNR of RED channel of image "<<fn<<" is "<<psnr_r<<endl;
	} else cout<<"The RED channel of image "<<fn<<" has no noise and MSE = "<<MSE_R<<endl;
	if (MSE_G != 0) {
		psnr_g =  10*log10(255*255/(MSE_G/SizeR/SizeV));
		cout<<"PSNR of GREEN channel of image "<<fn<<" is "<<psnr_g<<endl;
	} else cout<<"The GREEN channel of image "<<fn<<" has no noise and MSE = "<<MSE_G<<endl;
	if (MSE_B != 0) {
		psnr_b =  10*log10(255*255/(MSE_B/SizeR/SizeV));
		cout<<"PSNR of BLUE channel of image "<<fn<<" is "<<psnr_b<<endl;
	} else cout<<"The BLUE channel of image "<<fn<<" has no noise and MSE = "<<MSE_B<<endl;

	return 0;
}

int NLM_filter(int winsize, int winsize_ser, double h, unsigned char *ImageData, unsigned char *ImageData_New, int channel, int SizeV, int SizeR, int BytesPerPixel)
{
	int row = 0, col = 0, cha = 0, dev = 1;
	double sum = 0.0, dist = 0.0, Weight = 0.0;
	double mask[winsize][winsize];

	int **Data = new int*[SizeR+(winsize-1)];
	double **Data_new = new double*[SizeR];

	cout<<"Begin!"<<endl;
	if (winsize%2 != 1) {
		cout<<"Mask size in mean_filter should be odd."<<endl;
		exit(1);
	}

	// calculate mask
	double sum1 = 0.0, sum2 = 0.0;

	for (int i=0; i<winsize; i++) {
		for (int j=0; j<winsize; j++) {
			mask[i][j] = exp(((i-winsize/2)*(i-winsize/2)+(j-winsize/2)*(j-winsize/2))/(-2.0)/dev/dev)/(2.0*PI*dev*dev);
			sum1 = sum1 + mask[i][j];
		}
	}
	for (int i=0; i<winsize; i++) {
		for (int j=0; j<winsize; j++) {
			mask[i][j] = mask[i][j]/sum1;
			sum2 = sum2 + mask[i][j];
		}
	} 

	for (int i=0; i<(SizeR+(winsize-1)); i++) {
		Data[i] = new int[SizeV+(winsize-1)];
	}
	for (int i=0; i<SizeR; i++) {
		Data_new[i] = new double[SizeV];
	}
	for (int i=0; i<SizeV*SizeR*BytesPerPixel; i++){
		row = i/BytesPerPixel/SizeV;
		col = i/BytesPerPixel%SizeV;
		cha = i%3;
		if (cha == 0 && channel == 0) Data[row+winsize/2][col+winsize/2] = ImageData[i];
		if (cha == 1 && channel == 1) Data[row+winsize/2][col+winsize/2] = ImageData[i];
		if (cha == 2 && channel == 2) Data[row+winsize/2][col+winsize/2] = ImageData[i];
	}
	for (int j=winsize/2; j<(SizeV+winsize/2); j++) {
		for (int i=0; i<winsize/2; i++) {
			Data[i][j] = Data[winsize/2][j];
		}
		for (int i=SizeR+winsize/2; i<(SizeR+winsize-1); i++) {
			Data[i][j] = Data[SizeR-1+winsize/2][j];
		}
	}
	for (int i=0; i<(SizeV+winsize-1); i++) {
		for (int j=0; j<winsize/2; j++) {
			Data[i][j] = Data[i][winsize/2];
		}
		for (int j=(SizeR+winsize/2); j<(SizeR+winsize-1); j++) {
			Data[i][j] = Data[i][SizeR-1+winsize/2];
		}
	}

	// boundary top-left
	for (int i=0; i<winsize_ser/2; i++){
		for (int j=0; j<winsize_ser/2; j++){
			Weight = 0.0;
			sum = 0.0;
			Data_new[i][j] = 0;
			for (int p=0; p<winsize_ser; p++) {
				for (int q=0; q<winsize_ser; q++) {
					dist = 0.0;
					for(int m=0; m<winsize; m++) {
						for(int n=0; n<winsize; n++) {
							dist = dist + mask[m][n]*(double)((Data[i+m][j+n]-Data[p+m][q+n])*(Data[i+m][j+n]-Data[p+m][q+n]));
						}
					}
					Weight = exp((-1.0)*dist/h/h);
					sum = sum + Weight;
					Data_new[i][j] = Data_new[i][j] + Weight*Data[p+winsize/2][q+winsize/2];
				}
			}
			Data_new[i][j] = Data_new[i][j]/sum;
		}
	}

	// boundary top-middle
	for (int i=0; i<winsize_ser/2; i++){
		for (int j=winsize_ser/2; j<SizeV-winsize_ser/2; j++){
			Weight = 0.0;
			sum = 0.0;
			Data_new[i][j] = 0;
			for (int p=0; p<winsize_ser; p++) {
				for (int q=j-winsize_ser/2; q<=j+winsize_ser/2; q++) {
					dist = 0.0;
					for(int m=0; m<winsize; m++) {
						for(int n=0; n<winsize; n++) {
							dist = dist + mask[m][n]*(double)((Data[i+m][j+n]-Data[p+m][q+n])*(Data[i+m][j+n]-Data[p+m][q+n]));
						}
					}
					Weight = exp((-1.0)*dist/h/h);
					sum = sum + Weight;
					Data_new[i][j] = Data_new[i][j] + Weight*Data[p+winsize/2][q+winsize/2];
				}
			}
			Data_new[i][j] = Data_new[i][j]/sum;
		}
	}

	// boundary right-top
	for (int i=0; i<winsize_ser/2; i++){
		for (int j=SizeV-winsize_ser/2; j<SizeV; j++){
			Weight = 0.0;
			sum = 0.0;
			Data_new[i][j] = 0;
			for (int p=0; p<winsize_ser; p++) {
				for (int q=SizeV-winsize_ser; q<SizeV; q++) {
					dist = 0.0;
					for(int m=0; m<winsize; m++) {
						for(int n=0; n<winsize; n++) {
							dist = dist + mask[m][n]*(double)((Data[i+m][j+n]-Data[p+m][q+n])*(Data[i+m][j+n]-Data[p+m][q+n]));
						}
					}
					Weight = exp((-1.0)*dist/h/h);
					sum = sum + Weight;
					Data_new[i][j] = Data_new[i][j] + Weight*Data[p+winsize/2][q+winsize/2];
				}
			}
			Data_new[i][j] = Data_new[i][j]/sum;
		}
	}

	// boundary left-middle
	for (int i=winsize_ser/2; i<SizeR-winsize_ser/2; i++){
		for (int j=0; j<winsize_ser/2; j++){
			Weight = 0.0;
			sum = 0.0;
			Data_new[i][j] = 0;
			for (int p=i-winsize_ser/2; p<=i+winsize_ser/2; p++) {
				for (int q=0; q<winsize_ser; q++) {
					dist = 0.0;
					for(int m=0; m<winsize; m++) {
						for(int n=0; n<winsize; n++) {
							dist = dist + mask[m][n]*(double)((Data[i+m][j+n]-Data[p+m][q+n])*(Data[i+m][j+n]-Data[p+m][q+n]));
						}
					}
					Weight = exp((-1.0)*dist/h/h);
					sum = sum + Weight;
					Data_new[i][j] = Data_new[i][j] + Weight*Data[p+winsize/2][q+winsize/2];
				}
			}
			Data_new[i][j] = Data_new[i][j]/sum;
		}
	}

	// middle-middle
	for (int i=winsize_ser/2; i<SizeR-winsize_ser/2; i++){
		for (int j=winsize_ser/2; j<SizeV-winsize_ser/2; j++){
			Weight = 0.0;
			sum = 0.0;
			Data_new[i][j] = 0;
			for (int p=i-winsize_ser/2; p<=i+winsize_ser/2; p++) {
				for (int q=j-winsize_ser/2; q<=j+winsize_ser/2; q++) {
					dist = 0.0;
					for(int m=0; m<winsize; m++) {
						for(int n=0; n<winsize; n++) {
							dist = dist + mask[m][n]*(double)((Data[i+m][j+n]-Data[p+m][q+n])*(Data[i+m][j+n]-Data[p+m][q+n]));
						}
					}
					Weight = exp((-1.0)*dist/h/h);
					sum = sum + Weight;
					Data_new[i][j] = Data_new[i][j] + Weight*Data[p+winsize/2][q+winsize/2];
				}
			}
			Data_new[i][j] = Data_new[i][j]/sum;
		}
	}

	// boundary right-middle
	for (int i=winsize_ser/2; i<SizeR-winsize_ser/2; i++){
		for (int j=SizeV-winsize_ser/2; j<SizeV; j++){
			Weight = 0.0;
			sum = 0.0;
			Data_new[i][j] = 0;
			for (int p=i-winsize_ser/2; p<=i+winsize_ser/2; p++) {
				for (int q=SizeV-winsize_ser; q<SizeV; q++) {
					dist = 0.0;
					for(int m=0; m<winsize; m++) {
						for(int n=0; n<winsize; n++) {
							dist = dist + mask[m][n]*(double)((Data[i+m][j+n]-Data[p+m][q+n])*(Data[i+m][j+n]-Data[p+m][q+n]));
						}
					}
					Weight = exp((-1.0)*dist/h/h);
					sum = sum + Weight;
					Data_new[i][j] = Data_new[i][j] + Weight*Data[p+winsize/2][q+winsize/2];
				}
			}
			Data_new[i][j] = Data_new[i][j]/sum;
		}
	}
	
	// boundary left-bottom
	for (int i=SizeR-winsize_ser/2; i<SizeR; i++){
		for (int j=0; j<winsize_ser/2; j++){
			Weight = 0.0;
			sum = 0.0;
			Data_new[i][j] = 0;
			for (int p=SizeR-winsize_ser; p<SizeR; p++) {
				for (int q=0; q<winsize_ser; q++) {
					dist = 0.0;
					for(int m=0; m<winsize; m++) {
						for(int n=0; n<winsize; n++) {
							dist = dist + mask[m][n]*(double)((Data[i+m][j+n]-Data[p+m][q+n])*(Data[i+m][j+n]-Data[p+m][q+n]));
						}
					}
					Weight = exp((-1.0)*dist/h/h);
					sum = sum + Weight;
					Data_new[i][j] = Data_new[i][j] + Weight*Data[p+winsize/2][q+winsize/2];
				}
			}
			Data_new[i][j] = Data_new[i][j]/sum;
		}
	}

	// boundary middle-bottom
	for (int i=SizeR-winsize_ser/2; i<SizeR; i++){
		for (int j=winsize_ser/2; j<SizeV-winsize_ser/2; j++){
			Weight = 0.0;
			sum = 0.0;
			Data_new[i][j] = 0;
			for (int p=SizeR-winsize_ser; p<SizeR; p++) {
				for (int q=j-winsize_ser/2; q<=j+winsize_ser/2; q++) {
					dist = 0.0;
					for(int m=0; m<winsize; m++) {
						for(int n=0; n<winsize; n++) {
							dist = dist + mask[m][n]*(double)((Data[i+m][j+n]-Data[p+m][q+n])*(Data[i+m][j+n]-Data[p+m][q+n]));
						}
					}
					Weight = exp((-1.0)*dist/h/h);
					sum = sum + Weight;
					Data_new[i][j] = Data_new[i][j] + Weight*Data[p+winsize/2][q+winsize/2];
				}
			}
			Data_new[i][j] = Data_new[i][j]/sum;
		}
	}

	// boundary right-bottom
	for (int i=SizeR-winsize_ser/2; i<SizeR; i++){
		for (int j=SizeV-winsize_ser/2; j<SizeV; j++){
			Weight = 0.0;
			sum = 0.0;
			Data_new[i][j] = 0;
			for (int p=SizeR-winsize_ser; p<SizeR; p++) {
				for (int q=SizeV-winsize_ser; q<SizeV; q++) {
					dist = 0.0;
					for(int m=0; m<winsize; m++) {
						for(int n=0; n<winsize; n++) {
							dist = dist + mask[m][n]*(double)((Data[i+m][j+n]-Data[p+m][q+n])*(Data[i+m][j+n]-Data[p+m][q+n]));
						}
					}
					Weight = exp((-1.0)*dist/h/h);
					sum = sum + Weight;
					Data_new[i][j] = Data_new[i][j] + Weight*Data[p+winsize/2][q+winsize/2];
				}
			}
			Data_new[i][j] = Data_new[i][j]/sum;
		}
	}	


	for (int i=0; i<SizeV*SizeR*BytesPerPixel; i++){
		row = i/BytesPerPixel/SizeV;
		col = i/BytesPerPixel%SizeV;
		cha = i%3;
		if (cha == 0 && channel == 0) {
			ImageData_New[i] = round(Data_new[row][col]);
		} else if (cha == 1 && channel == 1) {
			ImageData_New[i] = round(Data_new[row][col]);
		} else if (cha == 2 && channel == 2) {
			ImageData_New[i] = round(Data_new[row][col]);
		}
	}
	
	for (int i=0; i<SizeR+(winsize-1); i++) {
		delete [] Data[i];
	}
	for (int i=0; i<SizeR; i++) {
		delete [] Data_new[i];
	}
	delete [] Data;
	delete [] Data_new;

	return 0;
}

int main(int argc, char *argv[])
{
	int BytesPerPixel = 3;
	int SizeV = 512, SizeR = 512;
	char *filename = new char[30];
	int occu[3][256];

	if (argc < 5 || (argc>5 && argc<8)){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name pepper_noisy pepper sailboat_noisy sailboat [VolumeSize = 512] [RowSize = 512] [BytesPerPixel = 3]" << endl;
		return 0;
	} else if (argc > 5) {
		SizeV = atoi(argv[5]);
		SizeR = atoi(argv[6]);
		BytesPerPixel = atoi(argv[7]);
	}

	unsigned char *ImageData = new unsigned char[SizeR*SizeV*BytesPerPixel];
	unsigned char *ImageData_New = new unsigned char[SizeR*SizeV*BytesPerPixel];
	unsigned char *ImageData_ORI = new unsigned char[SizeR*SizeV*BytesPerPixel];

	unsigned char *ImageDataS = new unsigned char[SizeR*SizeV*BytesPerPixel];
	unsigned char *ImageData_NewS = new unsigned char[SizeR*SizeV*BytesPerPixel];
	unsigned char *ImageData_ORIS = new unsigned char[SizeR*SizeV*BytesPerPixel];


	// pepper_noisy
	read_image(argv[1], SizeV, SizeR, BytesPerPixel, ImageData);
	read_image(argv[2], SizeV, SizeR, BytesPerPixel, ImageData_ORI);

	cal_occu(argv[1], SizeV, SizeR, BytesPerPixel, ImageData, occu);
	cal_psnr(argv[1], ImageData, ImageData_ORI, SizeV, SizeR, BytesPerPixel);

	// Use NLM_filter on 3 channels
	NLM_filter(5, 11, 35.0, ImageData, ImageData_New, 0, SizeV, SizeR, BytesPerPixel);
	NLM_filter(3, 6, 15.0, ImageData, ImageData_New, 1, SizeV, SizeR, BytesPerPixel);
	NLM_filter(3, 11, 70.0, ImageData, ImageData_New, 2, SizeV, SizeR, BytesPerPixel);

	memset(filename, 0, sizeof(filename));
	strcat(filename, "NLM_filter");
	write_image(argv[1], filename, SizeV, SizeR, BytesPerPixel, ImageData_New);

	cal_psnr(filename, ImageData_New, ImageData_ORI, SizeV, SizeR, BytesPerPixel);

	// sailboat_noisy
	read_image(argv[3], SizeV, SizeR, BytesPerPixel, ImageDataS);
	read_image(argv[4], SizeV, SizeR, BytesPerPixel, ImageData_ORIS);

	cal_occu(argv[3], SizeV, SizeR, BytesPerPixel, ImageDataS, occu);
	cal_psnr(argv[3], ImageDataS, ImageData_ORIS, SizeV, SizeR, BytesPerPixel);

	// Use NLM_filter on 3 channels
	NLM_filter(3, 11, 35.0, ImageDataS, ImageData_NewS, 0, SizeV, SizeR, BytesPerPixel);
	NLM_filter(3, 11, 30.0, ImageDataS, ImageData_NewS, 1, SizeV, SizeR, BytesPerPixel);
	NLM_filter(3, 11, 30.0, ImageDataS, ImageData_NewS, 2, SizeV, SizeR, BytesPerPixel);

	memset(filename, 0, sizeof(filename));
	strcat(filename, "NLM_filter");
	write_image(argv[3], filename, SizeV, SizeR, BytesPerPixel, ImageData_NewS);

	cal_psnr(filename, ImageData_NewS, ImageData_ORIS, SizeV, SizeR, BytesPerPixel);

	delete [] filename;
	delete [] ImageData;
	delete [] ImageData_New;
	delete [] ImageData_ORI;
	delete [] ImageDataS;
	delete [] ImageData_NewS;
	delete [] ImageData_ORIS;

	return 0;
}