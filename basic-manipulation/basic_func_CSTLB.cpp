//USC EE569 Homework-1 Problem-1-(b)(c) by Xuejing Lei
//Color space transformation and Layer blending mode implementation
//Email: xjlei1117@gmail.com
//Date: February 5, 2017

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>

using namespace std;

int max(int R, int G, int B) 
{
	int maxnum = R;
	if (maxnum < G) maxnum = G;
	if (maxnum < B) maxnum = B;
	return maxnum;
}

int min(int R, int G, int B)
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

	M1 = max(R, G, B);
	M2 = min(R, G, B);
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
	M1 = max(R, G, B);
	M2 = min(R, G, B);
	L = (M1 + M2) / 2.0 / 255.0;

	return L;
}

float cal_S(int R, int G, int B)
{
	int M1 = 0, M2 = 0, C = 0; 
	float L = 0.0, S = 0.0;

	M1 = max(R, G, B);
	M2 = min(R, G, B);
	L = (M1 + M2) / 2.0 / 255.0;
	C = (M1 - M2);

	if (L == 0 || L == 1) { S = 0; }
	else if (L > 0 && L < 0.5) { S = (float)C/(float)(M1+M2);}
	else S = C / 255.0/(2 - (M1+M2)/255.0);

	return S;
}

int RGB2CMY(char* fn_prefix, int SizeV, int SizeR, int BytesPerPixel)
{
	char *filename = new char[30];
	FILE *file, *file1, *file2, *file3;
	int n=0;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, "_Cyan.raw");
	if (!(file1=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, "_Magenta.raw");
	if (!(file2=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, "_Yellow.raw");
	if (!(file3=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}

	unsigned char ***ImageData = new unsigned char **[SizeR];
	unsigned char **Cyan = new unsigned char*[SizeR];
	unsigned char **Magenta = new unsigned char*[SizeR];
	unsigned char **Yellow = new unsigned char*[SizeR];

	for (int i=0; i<SizeR; i++) {
		ImageData[i] = new unsigned char *[SizeV];
		Cyan[i] = new unsigned char[SizeV];
		Magenta[i] = new unsigned char[SizeV];
		Yellow[i] = new unsigned char[SizeV];
		for (int j=0; j<SizeV; j++) {
			ImageData[i][j] = new unsigned char[BytesPerPixel];
			fread(ImageData[i][j], sizeof(unsigned char), BytesPerPixel, file);
			//cout<<(int)ImageData[i][j][0]<<","<<(int)ImageData[i][j][1]<<","<<(int)ImageData[i][j][2]<<" ";
			Cyan[i][j] = 255 - ImageData[i][j][0];
			Magenta[i][j] = 255 - ImageData[i][j][1];
			Yellow[i][j] = 255 - ImageData[i][j][2];
		}
		//write to file
		fwrite(Cyan[i], sizeof(unsigned char), SizeV, file1);
		fwrite(Magenta[i], sizeof(unsigned char), SizeV, file2);
		fwrite(Yellow[i], sizeof(unsigned char), SizeV, file3);
	}
	fclose(file);
	fclose(file1);
	fclose(file2);
	fclose(file3);

	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			delete [] ImageData[i][j];
		}
	}
	for (int i=0; i<SizeR; i++) {
		delete [] ImageData[i];
		delete [] Cyan[i];
		delete [] Magenta[i];
		delete [] Yellow[i];
	}
	delete [] ImageData;
	delete [] Cyan;
	delete [] Magenta;
	delete [] Yellow;
	delete [] filename;

	return 0;
}

int RGB2HSL(char* fn_prefix, int SizeV, int SizeR, int BytesPerPixel)
{
	char *filename = new char[30];
	FILE *file, *file1, *file2, *file3;
	int n=0;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, "_Hue.raw");
	if (!(file1=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, "_Saturation.raw");
	if (!(file2=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, "_Lightness.raw");
	if (!(file3=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}

	unsigned char ***ImageData = new unsigned char **[SizeR];
	unsigned char **Hue = new unsigned char*[SizeR];
	unsigned char **Satu = new unsigned char*[SizeR];
	unsigned char **Light = new unsigned char*[SizeR];

	for (int i=0; i<SizeR; i++) {
		ImageData[i] = new unsigned char *[SizeV];
		Hue[i] = new unsigned char[SizeV];
		Satu[i] = new unsigned char[SizeV];
		Light[i] = new unsigned char[SizeV];
		for (int j=0; j<SizeV; j++) {
			ImageData[i][j] = new unsigned char[BytesPerPixel];
			fread(ImageData[i][j], sizeof(unsigned char), BytesPerPixel, file);
			//cout<<(int)ImageData[i][j][0]<<","<<(int)ImageData[i][j][1]<<","<<(int)ImageData[i][j][2]<<" ";
			Hue[i][j] = round(cal_H((int)ImageData[i][j][0], (int)ImageData[i][j][1], (int)ImageData[i][j][2]) * 255.0 / 360.0);
			Satu[i][j] = cal_S((int)ImageData[i][j][0], (int)ImageData[i][j][1], (int)ImageData[i][j][2]) * 255;
			Light[i][j] = cal_L((int)ImageData[i][j][0], (int)ImageData[i][j][1], (int)ImageData[i][j][2]) * 255;
		}
		//write to file
		fwrite(Hue[i], sizeof(unsigned char), SizeV, file1);
		fwrite(Satu[i], sizeof(unsigned char), SizeV, file2);
		fwrite(Light[i], sizeof(unsigned char), SizeV, file3);
	}
	fclose(file);
	fclose(file1);
	fclose(file2);
	fclose(file3);

	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			delete [] ImageData[i][j];
		}
	}
	for (int i=0; i<SizeR; i++) {
		delete [] ImageData[i];
		delete [] Hue[i];
		delete [] Satu[i];
		delete [] Light[i];
	}
	delete [] ImageData;
	delete [] Hue;
	delete [] Satu;
	delete [] Light;
	delete [] filename;

	return 0;
}

int sepia_filter(char* fn_prefix, int SizeV, int SizeR, int BytesPerPixel)
{
	char *filename = new char[30];
	FILE *file, *file1, *file2;
	int n=0;
	
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, "_Gray.raw");
	if (!(file1=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, fn_prefix);
	strcat(filename, "_SepiaFilter.raw");
	if (!(file2=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	unsigned char ***ImageData = new unsigned char **[SizeR];
	unsigned char **Gray = new unsigned char*[SizeR];
	unsigned char ***ImageData_SF = new unsigned char **[SizeR];

	double temp1 = 0.0, temp2 = 0.0, temp3 = 0.0;

	for (int i=0; i<SizeR; i++) {
		ImageData[i] = new unsigned char *[SizeV];
		Gray[i] = new unsigned char[SizeV];
		ImageData_SF[i] = new unsigned char *[SizeV];
		for (int j=0; j<SizeV; j++) {
			ImageData[i][j] = new unsigned char[BytesPerPixel];
			fread(ImageData[i][j], sizeof(unsigned char), BytesPerPixel, file);
			ImageData_SF[i][j] = new unsigned char[BytesPerPixel];
			Gray[i][j] = 0.21*ImageData[i][j][0] + 0.72*ImageData[i][j][1] + 0.07*ImageData[i][j][2];
			temp1 = (0.393+0.769+0.189) * (double)Gray[i][j];
			temp2 = (0.349+0.686+0.168) * (double)Gray[i][j];
			temp3 = (0.272+0.534+0.131) * (double)Gray[i][j];
			if (temp1 >= 0 && temp1 <= 255) ImageData_SF[i][j][0] = round(temp1);
			else if (temp1 > 255) ImageData_SF[i][j][0] = 255;
			else if (temp1 < 0) ImageData_SF[i][j][0] = 0;
			if (temp2 >= 0 && temp2 <= 255) ImageData_SF[i][j][1] = round(temp2);
			else if (temp2 > 255) ImageData_SF[i][j][1] = 255;
			else if (temp2 < 0) ImageData_SF[i][j][1] = 0;
			if (temp3 >= 0 && temp3 <= 255) ImageData_SF[i][j][2] = round(temp3);
			else if (temp3 > 255) ImageData_SF[i][j][2] = 255;
			else if (temp3 < 0) ImageData_SF[i][j][2] = 0;
			
			fwrite(ImageData_SF[i][j], sizeof(unsigned char), BytesPerPixel, file2);
		}
		fwrite(Gray[i], sizeof(unsigned char), SizeV, file1);
	}
	fclose(file);
	fclose(file1);
	fclose(file2);

	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			delete [] ImageData[i][j];
			delete [] ImageData_SF[i][j];
		}
	}
	for (int i=0; i<SizeR; i++) {
		delete [] ImageData[i];
		delete [] ImageData_SF[i];
		delete [] Gray[i];
	}
	delete [] ImageData;
	delete [] ImageData_SF;
	delete [] Gray;
	delete [] filename;

	return 0;
}

int screen_blend(char* top_pre, char* bot_pre, int SizeV, int SizeR, int BytesPerPixel)
{
	char *filename = new char[30];
	FILE *file, *file1, *file2;
	int n=0;
	float temp1 = 0.0, temp2 = 0.0;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, top_pre);
	strcat(filename, ".raw");
	if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, bot_pre);
	strcat(filename, ".raw");
	if (!(file1=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	memset(filename, 0, sizeof(filename));
	strcpy(filename, "Images");
	strcat(filename, "_Blend.raw");
	if (!(file2=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	unsigned char ***ImageData_top = new unsigned char **[SizeR];
	unsigned char ***ImageData_bot = new unsigned char **[SizeR];
	unsigned char ***ImageData_fin = new unsigned char **[SizeR];
	
	for (int i=0; i<SizeR; i++) {
		ImageData_top[i] = new unsigned char *[SizeV];
		ImageData_bot[i] = new unsigned char *[SizeV];
		ImageData_fin[i] = new unsigned char *[SizeV];
		for (int j=0; j<SizeV; j++) {
			ImageData_top[i][j] = new unsigned char[BytesPerPixel];
			ImageData_bot[i][j] = new unsigned char[BytesPerPixel];
			ImageData_fin[i][j] = new unsigned char[BytesPerPixel];
			fread(ImageData_top[i][j], sizeof(unsigned char), BytesPerPixel, file);
			fread(ImageData_bot[i][j], sizeof(unsigned char), BytesPerPixel, file1);	
			for (int k=0; k<BytesPerPixel; k++) {
				temp1 = (int)ImageData_bot[i][j][k]/255.0;
				temp2 = (int)ImageData_top[i][j][k]/255.0;
				ImageData_fin[i][j][k] = round((1 - (1-temp1)*(1-temp2))* 255);
			}
			fwrite(ImageData_fin[i][j], sizeof(unsigned char), BytesPerPixel, file2);
		}
	}
	fclose(file);
	fclose(file1);
	fclose(file2);

	for (int i=0; i<SizeR; i++) {
		for (int j=0; j<SizeV; j++) {
			delete [] ImageData_top[i][j];
			delete [] ImageData_bot[i][j];
			delete [] ImageData_fin[i][j];
		}
	}
	for (int i=0; i<SizeR; i++) {
		delete [] ImageData_top[i];
		delete [] ImageData_bot[i];
		delete [] ImageData_fin[i];
	}
	delete [] ImageData_top;
	delete [] ImageData_bot;
	delete [] ImageData_fin;
	delete [] filename;

	return 0;
}

int main(int argc, char *argv[])
{
	int BytesPerPixel = 3; //color image
	int SizeV_Pa = 512, SizeR_Pa = 384; // size of parrot.raw
	int SizeV_Bu = 789, SizeR_Bu = 600; //size of building.raw
	int SizeV_Ca = 600, SizeR_Ca = 398; // size of cat.raw
	int SizeV_Do = 640, SizeR_Do = 480; // size of dolphin.raw
	int SizeV_Be = 1914, SizeR_Be = 808; // size of beach.raw
	int SizeV_Bl = 890, SizeR_Bl = 900; // size of beach.raw

	// Check for proper syntax
	if (argc < 7){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name parrot building cat dolphin beach top_layer bottom_layer" << endl;
		return 0;
	}
	
	// CMY color space
	RGB2CMY(argv[1], SizeV_Pa, SizeR_Pa, BytesPerPixel);
	RGB2CMY(argv[2], SizeV_Bu, SizeR_Bu, BytesPerPixel);

	// HSL color space
	RGB2HSL(argv[3], SizeV_Ca, SizeR_Ca, BytesPerPixel);
	RGB2HSL(argv[4], SizeV_Do, SizeR_Do, BytesPerPixel);

	// Sepia Filter
	sepia_filter(argv[5], SizeV_Be, SizeR_Be, BytesPerPixel);

	// Screen blend
	screen_blend(argv[6], argv[7], SizeV_Bl, SizeR_Bl, BytesPerPixel);

	return 0;
}
