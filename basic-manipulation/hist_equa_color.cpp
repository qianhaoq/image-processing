//USC EE569 Homework-1 Problem-2-(b) by Xuejing Lei
//Three methods of histogram equalization for color images
//Method A: the linear transfer-function-based histogram enhancement method
//Method B: the cumulative-probability-based histogram equalization method
//Method C: preserve both hue and saturation histogram distribution and equalize brightness
//Email: xjlei1117@gmail.com
//Date: February 5, 2017

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fstream>

using namespace std;

int max2(int a, int b)
{
	int result = 0;
	if (a<b) result = b;
	else result = a;
	return result;
}

int min2(int a, int b)
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

int linear_trans_func_color(char *fn_pre, int SizeV, int SizeR)
{
	FILE *file, *file1;
	char *filename = new char[30];
	int min_r = 255, max_r = 0, min_g = 255, max_g = 0, min_b = 255, max_b = 0;
	int BytesPerPixel = 3;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}

	unsigned char ***ImageData = new unsigned char**[SizeR];
	unsigned char ***ImageData_New = new unsigned char**[SizeR];
	int *occu_r = new int[256];
	int *occu_g = new int[256];
	int *occu_b = new int[256];
	memset(occu_r, 0, 256*sizeof(int));
	memset(occu_g, 0, 256*sizeof(int));
	memset(occu_b, 0, 256*sizeof(int));

	for (int i=0; i<SizeR; i++) {
		ImageData[i] = new unsigned char*[SizeV];
		ImageData_New[i] = new unsigned char*[SizeV];
		for (int j=0; j<SizeV; j++) {
			ImageData[i][j] = new unsigned char[BytesPerPixel];
			ImageData_New[i][j] = new unsigned char[BytesPerPixel];
			fread(ImageData[i][j], sizeof(unsigned char), BytesPerPixel, file);
			occu_r[((int)ImageData[i][j][0])]++;
			max_r = max2(max_r, (int)ImageData[i][j][0]);
			min_r = min2(min_r, (int)ImageData[i][j][0]);
			occu_g[((int)ImageData[i][j][1])]++;
			max_g = max2(max_g, (int)ImageData[i][j][1]);
			min_g = min2(min_g, (int)ImageData[i][j][1]);
			occu_b[((int)ImageData[i][j][2])]++;
			max_b = max2(max_b, (int)ImageData[i][j][2]);
			min_b = min2(min_b, (int)ImageData[i][j][2]);
		}	
	}
	fclose(file);

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_occurance_A.txt");
	ofstream fout(filename);
	fout<<"Red channel:\n";
	for (int i=0; i<256; i++) {
		fout<<occu_r[i]<<"\n";
	}
	fout<<"Green channel:\n";
	for (int i=0; i<256; i++) {
		fout<<occu_g[i]<<"\n";
	}
	fout<<"Blue channel:\n";
	for (int i=0; i<256; i++) {
		fout<<occu_b[i]<<"\n";
	}
	fout.close();

	int sum_r = 0, sum_g = 0, sum_b = 0;
	float rate = 0.05;
	for (int i=0; i<256; i++) {

		sum_r = sum_r +occu_r[255-i];
		sum_g = sum_g +occu_g[255-i];
		sum_b = sum_b +occu_b[255-i];
		if (sum_r > SizeV*SizeR*rate && (sum_r-occu_r[255-i]) < SizeR*SizeV*rate) {
			max_r = 255-i;
			//min_r = i;
		}
		if (sum_g > SizeV*SizeR*rate && (sum_g-occu_g[255-i]) < SizeR*SizeV*rate) {
			max_g = 255-i;
			//min_g = i;
		}
		if (sum_b > SizeV*SizeR*rate && (sum_b-occu_b[255-i]) < SizeR*SizeV*rate) {
			max_b = 255-i;
			//min_b = i;
		}
	}
	
	
	cout<<"Method A: Range of RED values in original image "<<fn_pre<<".raw is ["<<min_r<<", "<<max_r<<"]."<<endl;
	cout<<"Method A: Range of GREEN values in original image "<<fn_pre<<".raw is ["<<min_g<<", "<<max_g<<"]."<<endl;
	cout<<"Method A: Range of BLUE values in original image "<<fn_pre<<".raw is ["<<min_b<<", "<<max_b<<"]."<<endl;


	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_MethodA.raw");
	if (!(file1=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			if ((int)ImageData[i][j][0] >= min_r && (int)ImageData[i][j][0] <= max_r)
				ImageData_New[i][j][0] = linear_trans_func(ImageData[i][j][0], min_r, max_r, 0, 255);
			else ImageData_New[i][j][0] = 255;

			if ((int)ImageData[i][j][1] >= min_g && (int)ImageData[i][j][1] <= max_g)
				ImageData_New[i][j][1] = linear_trans_func(ImageData[i][j][1], min_g, max_g, 0, 255);
			else ImageData_New[i][j][1] = 255;

			if ((int)ImageData[i][j][2] >= min_b && (int)ImageData[i][j][2] <= max_b) 
				ImageData_New[i][j][2] = linear_trans_func(ImageData[i][j][2], min_b, max_b, 0, 255);
			else ImageData_New[i][j][2] = 255;

			fwrite(ImageData_New[i][j], sizeof(unsigned char), BytesPerPixel, file1);
		}
	}
	fclose(file1);

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
	delete [] filename;
	delete [] occu_r;
	delete [] occu_g;
	delete [] occu_b;

	return 0;
}

int cdf_trans_func(int cdf, int min_cdf, int max_cdf)
{
	float k = 0.0;
	k = 255.0/(max_cdf - min_cdf);
	return round(k*(cdf - min_cdf));
}

int get_cdf(char *fn_pre, int cdf[], int occu[], int min, int max, char *flag)
{
	int sum = 0;
	char *filename = new char[30];

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_record_");
	strcat(filename, flag);
	strcat(filename, "_B.txt");
	ofstream fout(filename);
	fout<<"Occurance of "<<flag <<" channel in "<<fn_pre<<".raw:"<<"\n";
	for (int i=0; i<256; i++) {
		if (i >= min && i <= max) {
			if (occu[i] != 0) {
				if (i == min) cdf[i] = occu[i];
				else cdf[i] = sum +occu[i];
				sum = cdf[i];
			} else {
				cdf[i] = 0;
			}
		} else {
			cdf[i] = 0;
		}
		fout<<occu[i]<<"\n";
	}
	fout<<"\n\nCDF of "<<flag <<" channel in "<<fn_pre<<".raw:"<<"\n";
	for (int i=0; i<256; i++) fout<<cdf[i]<<"\n";
	fout<<"\n\nNew_Intensity of "<<flag <<" channel in "<<fn_pre<<".raw:"<<"\n";
	for (int i=0; i<256; i++) fout<<cdf_trans_func(cdf[i], cdf[min], cdf[max])<<"\n";
	fout.close();
	
	if (strcmp(flag, "LIGHT") == 0) cout<<"Method C: Range of values in original image "<<fn_pre<<".raw is ["<<min<<", "<<max<<"]."<<endl;
	else cout<<"Method B: Range of "<<flag<<" values in original image "<<fn_pre<<".raw is ["<<min<<", "<<max<<"]."<<endl;
	
	delete [] filename;
	return 0;
}

int cdf_trans_color(char *fn_pre, int SizeV, int SizeR)
{
	FILE *file, *file1;
	char *filename = new char[30];
	char *temp = new char[10];
	int min_r = 255, max_r = 0, min_g = 255, max_g = 0, min_b = 255, max_b = 0;
	int BytesPerPixel = 3;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	} 

	unsigned char ***ImageData = new unsigned char**[SizeR];
	unsigned char ***ImageData_New = new unsigned char**[SizeR];
	int *occu_r = new int[256];
	int *occu_g = new int[256];
	int *occu_b = new int[256];
	int *cdf_r = new int[256];
	int *cdf_g = new int[256];
	int *cdf_b = new int[256];
	memset(occu_r, 0, 256*sizeof(int));
	memset(occu_g, 0, 256*sizeof(int));
	memset(occu_b, 0, 256*sizeof(int));
	memset(cdf_r, 0, 256*sizeof(int));
	memset(cdf_g, 0, 256*sizeof(int));
	memset(cdf_b, 0, 255*sizeof(int));

	for (int i=0; i<SizeR; i++) {
		ImageData[i] = new unsigned char*[SizeV];
		ImageData_New[i] = new unsigned char*[SizeV];
		for (int j=0; j<SizeV; j++) {
			ImageData[i][j] = new unsigned char[BytesPerPixel];
			ImageData_New[i][j] = new unsigned char[BytesPerPixel];
			fread(ImageData[i][j], sizeof(unsigned char), BytesPerPixel, file);
			occu_r[((int)ImageData[i][j][0])]++;
			max_r = max2(max_r, ImageData[i][j][0]);
			min_r = min2(min_r, ImageData[i][j][0]);
			occu_g[((int)ImageData[i][j][1])]++;
			max_g = max2(max_g, ImageData[i][j][1]);
			min_g = min2(min_g, ImageData[i][j][1]);
			occu_b[((int)ImageData[i][j][2])]++;
			max_b = max2(max_b, ImageData[i][j][2]);
			min_b = min2(min_b, ImageData[i][j][2]);
		}
	}
	fclose(file);
	
	memset(temp, 0, sizeof(temp));
	strcpy(temp, "RED");
	get_cdf(fn_pre, cdf_r, occu_r, min_r, max_r, temp);
	memset(temp, 0, sizeof(temp));
	strcpy(temp, "GREEN");
	get_cdf(fn_pre, cdf_g, occu_g, min_g, max_g, temp);
	memset(temp, 0, sizeof(temp));
	strcpy(temp, "BLUE");
	get_cdf(fn_pre, cdf_b, occu_b, min_b, max_b, temp);

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_MethodB.raw");
	if (!(file1=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			ImageData_New[i][j][0] = cdf_trans_func(cdf_r[(int)ImageData[i][j][0]], cdf_r[min_r], cdf_r[max_r]);
			ImageData_New[i][j][1] = cdf_trans_func(cdf_g[(int)ImageData[i][j][1]], cdf_g[min_g], cdf_g[max_g]);
			ImageData_New[i][j][2] = cdf_trans_func(cdf_b[(int)ImageData[i][j][2]], cdf_b[min_b], cdf_b[max_b]);
			fwrite(ImageData_New[i][j], sizeof(unsigned char), BytesPerPixel, file1);
		}
	}
	fclose(file1);

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
	delete [] filename;
	delete [] temp;
	delete [] occu_r;
	delete [] occu_g;
	delete [] occu_b;
	delete [] cdf_r;
	delete [] cdf_g;
	delete [] cdf_b;

	return 0;
}

int max3(int R, int G, int B) 
{
	int maxnum = R;
	if (maxnum < G) maxnum = G;
	if (maxnum < B) maxnum = B;
	return maxnum;
}

int min3(int R, int G, int B)
{
	int minnum = R;
	if (minnum > G) minnum = G;
	if (minnum > B) minnum = B;
	return minnum;
}

float modu(float a, int b)
{
	int k = 0;
	float res;

	k = floor(a/(float)b);
	res =a - b*k;

	return res;
}

int cal_H(int R, int G, int B)
{
	int M1 = 0, M2 = 0, C = 0;
	float H = 0.0;

	M1 = max3(R, G, B);
	M2 = min3(R, G, B);
	C = M1 - M2;

	if (C == 0) { H = 0; }
	else if (M1 == R) { H = 60*modu((float)(G - B)/(float)C, 6);}
	else if (M1 == G) { H = 60*(((float)(B - R)/(float)C) + 2); }
	else if (M1 == B) { H = 60*(((float)(R - G)/(float)C) + 4); }

	return round(H);
}

float cal_L(int R, int G, int B)
{
	int M1 = 0, M2 = 0;
	float L = 0.0;
	M1 = max3(R, G, B);
	M2 = min3(R, G, B);
	L = (M1 + M2) / 2.0 / 255.0;

	return L;
}

float cal_S(int R, int G, int B)
{
	int M1 = 0, M2 = 0, C = 0; 
	float L = 0.0, S = 0.0;

	M1 = max3(R, G, B);
	M2 = min3(R, G, B);
	L = (M1 + M2) / 2.0 / 255.0;
	C = (M1 - M2);

	if (L == 0 || L == 1) { S = 0; }
	else if (L > 0 && L < 0.5) { S = (float)C/(float)(M1+M2);}
	else S = C / 255.0/(2 - (M1+M2)/255.0);

	return S;
}

int cal_R(int H, float S, float L)
{
	float hh = 0, x = 0.0, c = 0.0, m = 0.0;

	c = (1 - fabs(2.0*L - 1.0))*S;
	hh = (float)H / 60.0;
	m = L-0.5*c;
	// cout<<"c = "<<c<<endl;

	if (hh>=0 && hh<=1) {
		x = c*(1-fabs(hh - 1));
		// cout<<"hh = "<<hh<<" x = "<<x<<endl;
		return round((c+m)*255.0);
	} else if (hh>1 && hh<2) {
		x= c*(1-fabs(hh - 1));
		return round((x+m)*255.0);
	} else if (hh>=2 && hh<3) {
		x = c*(1-fabs(hh - 2));
		return round((0+m)*255.0);
	} else if (hh>=3 && hh<4) {
		x = c*(1-fabs(hh - 2));
		return round((0+m)*255.0);
	} else if (hh>=4 && hh<5) {
		x = c*(1-fabs(hh - 4 - 1));
		return round((x+m)*255.0);
	} else if (hh>=5 && hh<6) {
		x = c*(1-fabs(hh - 4 - 1));
		return round((c+m)*255.0);
	} else return round(m*255.0);
}

int cal_G(int H, float S, float L)
{
	float hh = 0, x = 0.0, c = 0.0, m = 0.0;

	c = (1 - fabs(2.0*L - 1.0))*S;
	hh = (float)H / 60.0;
	m = L-0.5*c;

	if (hh>=0 && hh<=1) {
		x = c*(1-fabs(hh - 1));
		return round((x+m)*255.0);
	} else if (hh>1 && hh<2) {
		return round((c+m)*255.0);
	} else if (hh>=2 && hh<3) {
		return round((c+m)*255.0);
	} else if (hh>=3 && hh<4) {
		x = c*(1-fabs(hh - 2));
		return round((x+m)*255.0);
	} else if (hh>=4 && hh<5) {
		return round((0+m)*255.0);
	} else if (hh>=5 && hh<6) {
		return round((0+m)*255.0);
	} else return round(m*255.0);
}

int cal_B(int H, float S, float L)
{
	float hh = 0, x = 0.0, c = 0.0, m = 0.0;

	c = (1 - fabs(2.0*L - 1.0))*S;
	hh = (float)H / 60.0;
	m = L-0.5*c;

	if (hh>=0 && hh<=1) {
		return round((0+m)*255.0);
	} else if (hh>1 && hh<2) {
		return round((0+m)*255.0);
	} else if (hh>=2 && hh<3) {
		x = c*(1-fabs(hh - 2));
		return round((x+m)*255.0);
	} else if (hh>=3 && hh<4) {
		return round((c+m)*255.0);
	} else if (hh>=4 && hh<5) {
		return round((c+m)*255.0);
	} else if (hh>=5 && hh<6) {
		x = c*(1-fabs(hh - 4 - 1));
		return round((x+m)*255.0);
	} else return round(m*255.0);
}

int preserve_HS_method (char *fn_pre, int SizeV, int SizeR)
{
	char *filename = new char[30];
	char *temp = new char[10];
	FILE *file, *file1;
	int BytesPerPixel = 3, min_f = 255, max_f = 0, temp_h = 0;
	float temp_s = 0.0, temp_l = 0.0;

	// Read image
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}

	unsigned char ***ImageData = new unsigned char **[SizeR];
	unsigned char ***ImageData_HSL = new unsigned char **[SizeR];
	unsigned char ***ImageData_New = new unsigned char **[SizeR];
	int *occu = new int[256];
	int *cdf = new int[256];
	memset(occu, 0, 256*sizeof(int));
	memset(cdf, 0, 256*sizeof(int));

	for (int i=0; i<SizeR; i++) {
		ImageData[i] = new unsigned char *[SizeV];
		ImageData_HSL[i] = new unsigned char *[SizeV];
		ImageData_New[i] = new unsigned char *[SizeV];
		for (int j=0; j<SizeV; j++) {
			ImageData[i][j] = new unsigned char[BytesPerPixel];
			ImageData_HSL[i][j] = new unsigned char[BytesPerPixel];
			ImageData_New[i][j] = new unsigned char[BytesPerPixel];
			fread(ImageData[i][j], sizeof(unsigned char), BytesPerPixel, file);
			// RGB 2 HSL
			ImageData_HSL[i][j][0] = round(cal_H((int)ImageData[i][j][0], (int)ImageData[i][j][1], (int)ImageData[i][j][2])*255.0/360.0);
			ImageData_HSL[i][j][1] = cal_S((int)ImageData[i][j][0], (int)ImageData[i][j][1], (int)ImageData[i][j][2]) * 255;
			ImageData_HSL[i][j][2] = cal_L((int)ImageData[i][j][0], (int)ImageData[i][j][1], (int)ImageData[i][j][2]) * 255;
			occu[((int)ImageData_HSL[i][j][2])]++;
			max_f = max2(max_f, ImageData_HSL[i][j][2]);
			min_f = min2(min_f, ImageData_HSL[i][j][2]);
		}
	}
	fclose(file);

	memset(temp, 0, sizeof(temp));
	strcpy(temp, "LIGHT");
	get_cdf(fn_pre, cdf, occu, min_f, max_f, temp);

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_pre);
	strcat(filename, "_MethodC.raw");
	if (!(file1=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			temp_h = 0; temp_s = 0.0; temp_l = 0.0;
			temp_h = (int)ImageData_HSL[i][j][0]*360/255;
			temp_s = (float)((int)ImageData_HSL[i][j][1]) / 255.0;
			temp_l = (float)(cdf_trans_func(cdf[(int)ImageData_HSL[i][j][2]], cdf[min_f], cdf[max_f])) / 255.0;
			// HSL 2 RGB
			ImageData_New[i][j][0] = cal_R(temp_h, temp_s, temp_l);
			ImageData_New[i][j][1] = cal_G(temp_h, temp_s, temp_l);
			ImageData_New[i][j][2] = cal_B(temp_h, temp_s, temp_l);
			fwrite(ImageData_New[i][j], sizeof(unsigned char), BytesPerPixel, file1);
		}
	}
	fclose(file1);

	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			delete [] ImageData[i][j];
			delete [] ImageData_HSL[i][j];
			delete [] ImageData_New[i][j];
		}
	}
	for (int i=0; i<SizeR; i++) {
		delete [] ImageData[i];
		delete [] ImageData_HSL[i];
		delete [] ImageData_New[i];
	}
	delete [] ImageData;
	delete [] ImageData_HSL;
	delete [] ImageData_New;
	delete [] occu;
	delete [] cdf;
	delete [] filename;

	return 0;


}

int main(int argc, char *argv[])
{
	int BytesPerPixel = 3; //gray image
	int SizeV = 940, SizeR = 400; // size of image

	// Check for proper syntax
	if (argc < 2){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name bedroom [VolumeSize = 940] [RowSize = 400]" << endl;
		return 0;
	} else if (argc > 2) {
		SizeV = atoi(argv[2]);
		SizeR = atoi(argv[3]);
	} 
	
	linear_trans_func_color(argv[1], SizeV, SizeR);
	
	cdf_trans_color(argv[1], SizeV, SizeR);

	preserve_HS_method(argv[1], SizeV, SizeR);

	return 0;
}