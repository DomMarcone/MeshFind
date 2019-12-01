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
			return (int)(start-realStart)/stride;
		}
		
	}
	return -1;
}


int rtpFormatElementCount(std::string format){
	int result = 0;
	for(int i=0;i<format.size();++i){
		switch(format[i]){
			case 'x' :
			case 'y' :
			case 'z' :
			case 'u' :
			case 'v' : 
			case 'r' :
			case 'g' :
			case 'b' :
			case 'p' :
				//normals will be counted through x, y, and z
				result++;
				break;
		}
	}
	return result;
}


void rtpFillVector(std::vector<float> &fv, int offset, int stride, float *start, float *end){
	start += offset;
	for(;start<end;start+=stride)
		fv.push_back(*start);
}


void rtpFillMeshData(MeshData &md, std::string format, float *start, float *end){
	int stride = rtpFormatElementCount(format);
	
	for(int i=0;i<format.size();++i){
		switch(format[i]){
		case 'x' :
			rtpFillVector(md.x, i, stride, start, end);
			break;
			
		case 'y' :
			rtpFillVector(md.y, i, stride, start, end);
			break;
			
		case 'z' :
			rtpFillVector(md.z, i, stride, start, end);
			break;

		case 'n' : //normals
			i++;//what's the next character?
			switch(format[i]){
			case 'x' :
				rtpFillVector(md.nx, i, stride, start, end);
				break;
				
			case 'y' :
				rtpFillVector(md.ny, i, stride, start, end);
				break;
				
			case 'z' :
				rtpFillVector(md.nz, i, stride, start, end);
				break;
			}
			break;//END NORMALS
			
		case 'u' :
			rtpFillVector(md.u, i, stride, start, end);
			break;
			
		case 'v' :
			rtpFillVector(md.v, i, stride, start, end);
			break;
			
		case 'r' :
			rtpFillVector(md.r, i, stride, start, end);
			break;
			
		case 'g' :
			rtpFillVector(md.g, i, stride, start, end);
			break;
			
		case 'b' :
			rtpFillVector(md.b, i, stride, start, end);
			break;
		}
	}
}

void rtpExportMeshData(MeshData &md, std::string filename){
	std::ofstream outFile(filename);
	
	if(!outFile.is_open()){
		std::cout << "Error creating " << filename << " for export!" << std::endl;
		exit(-1);
	}
	
	//Create the file header...
	outFile << "ply" << std::endl;
	outFile << "format ascii 1.0" << std::endl;
	outFile << "comment Generated with MeshFind" << std::endl;
	outFile << "element vertex " << md.x.size() << std::endl;
	outFile << "property float x" << std::endl;
	outFile << "property float y" << std::endl;
	outFile << "property float z" << std::endl;
	
	if(md.nx.size())outFile << "property float nx" << std::endl;
	if(md.ny.size())outFile << "property float ny" << std::endl;
	if(md.nz.size())outFile << "property float nz" << std::endl;
	
	if(md.u.size())outFile << "property float s" << std::endl;
	if(md.v.size())outFile << "property float t" << std::endl;
	
	if(md.r.size())outFile << "property float red" << std::endl;
	if(md.g.size())outFile << "property float blue" << std::endl;
	if(md.b.size())outFile << "property float green" << std::endl;
	
	//todo
	
	outFile.close();
}

//overloads
int rtpExportRange(std::string filename, std::string format, float *start, float *end){
	MeshData md;
	//todo...
	rtpFillMeshData(md, format, start, end);

	rtpExportMeshData(md, filename);
	return 0;
}