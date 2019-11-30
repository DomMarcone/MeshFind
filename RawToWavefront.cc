//RawToWavefront.cc

#include "RawToWavefront.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


typedef struct {
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> z;
	std::vector<float> nx;
	std::vector<float> ny;
	std::vector<float> nz;
	std::vector<float> u;
	std::vector<float> v;
	std::vector<float> r;
	std::vector<float> g;
	std::vector<float> b;
} MeshData;

void rtwFillVector(std::vector<float> &fv, int offset, int stride, float *start, float *end){
	start += offset;
	for(;start<end;start+=stride)
		fv.push_back(*start);
}

int rtwFillMeshData(MeshData &md, std::string format, float *start, float *end){
	//todo...
	return 0;
}

//overloads
int rtwExportRange(std::string filename, std::string format, float *start, float *end){
	MeshData md;
	//todo...
	rtwFillMeshData(md, format, start, end);
	//export MeshData
	return 0;
}