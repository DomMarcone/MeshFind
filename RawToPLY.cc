//RawToPLY.cc

#include "RawToPLY.h"

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

void rtpFillVector(std::vector<float> &fv, int offset, int stride, float *start, float *end){
	start += offset;
	for(;start<end;start+=stride)
		fv.push_back(*start);
}

int rtpFindVertexElementIndex(float *a, int stride, float *start, float *end){
	float * realStart = start;
	for(start;start<end;start+=stride){
		bool matches = true;
		for(int i=0;i<stride;++i){
			if(a[i] != start[i]){
				matches = false;
				break;
			}
		}
		
		if(matches){
			return (int)(start-realStart);
		}
		
	}
	return -1;
}

int rtpFillMeshData(MeshData &md, std::string format, float *start, float *end){
	//todo...
	return 0;
}

//overloads
int rtpExportRange(std::string filename, std::string format, float *start, float *end){
	MeshData md;
	//todo...
	rtpFillMeshData(md, format, start, end);
	//export MeshData
	return 0;
}