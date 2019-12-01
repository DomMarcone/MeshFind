//RawToPLY.cc

#include "RawToPLY.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>


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


int rtpMeshDataElementCount(MeshData &md){
	int result = 3;
	
	if(md.nx.size())result++;
	if(md.ny.size())result++;
	if(md.nz.size())result++;
	
	if(md.u.size())result++;
	if(md.v.size())result++;
	
	if(md.r.size())result++;
	if(md.g.size())result++;
	if(md.b.size())result++;
	
	return result;
}

#define _MIN(A,B) ((A<B)?(A):(B))

int rtpMeshDataLength(MeshData &md){
	int r = md.x.size();
	r = _MIN(r, md.y.size());
	r = _MIN(r, md.z.size());
	
	if(md.nx.size())r = _MIN(r, md.nx.size());
	if(md.ny.size())r = _MIN(r, md.ny.size());
	if(md.nz.size())r = _MIN(r, md.nz.size());
	
	if(md.u.size())r = _MIN(r, md.u.size());
	if(md.v.size())r = _MIN(r, md.v.size());
	
	if(md.r.size())r = _MIN(r, md.r.size());
	if(md.g.size())r = _MIN(r, md.g.size());
	if(md.b.size())r = _MIN(r, md.b.size());
	
	return r;
}


bool rtpCompareMeshDataIndex(MeshData &md, int a, int b){
	int size = rtpMeshDataLength(md);	
	if(a<0 || a>=size)return false;
	if(b<0 || b>=size)return false;
	
	if(a == b)return true;
	
	if(md.x[a] != md.x[a])return false;
	if(md.y[a] != md.y[a])return false;
	if(md.z[a] != md.z[a])return false;
	
	if(md.nx.size()>0 && md.nx[a] != md.nx[b])return false;
	if(md.ny.size()>0 && md.ny[a] != md.ny[b])return false;
	if(md.nz.size()>0 && md.nz[a] != md.nz[b])return false;
	
	if(md.u.size()>0 && md.u[a] != md.u[b])return false;
	if(md.v.size()>0 && md.v[a] != md.v[b])return false;
	
	if(md.r.size()>0 && md.r[a] != md.r[b])return false;
	if(md.g.size()>0 && md.g[a] != md.g[b])return false;
	if(md.b.size()>0 && md.b[a] != md.b[b])return false;
	
	return true;
}


int rtpFindFirstIndex(MeshData &md, int index){
	if(index >= rtpMeshDataLength(md))return 0;

	for(int i=0;i<index;++i){
		if(rtpCompareMeshDataIndex(md,index,i)){
			return i;
		}
	}

	return index;
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

void rtpMeshDataRemoveDuplicates(MeshData &md){
	bool checkNx, checkNy, checkNz, checkU, checkV, checkR, checkG, checkB;
	
	checkNx = (bool)md.nx.size();
	checkNy = (bool)md.ny.size();
	checkNz = (bool)md.nz.size();
	checkU = (bool)md.u.size();
	checkV = (bool)md.v.size();
	checkR = (bool)md.r.size();
	checkG = (bool)md.g.size();
	checkB = (bool)md.b.size();
	
	//todo
	
}

void rtpExportMeshData(MeshData &md, std::string filename){
	std::ofstream outFile(filename);
	int mdSize = rtpMeshDataLength(md);
	
	if(!outFile.is_open()){
		std::cout << "Error creating " << filename << " for export!" << std::endl;
		exit(-1);
	}
	
	//Create the file header...
	outFile << "ply" << std::endl;
	outFile << "format ascii 1.0" << std::endl;
	outFile << "comment Generated with MeshFind" << std::endl;
	outFile << "element vertex " << mdSize << std::endl;
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
	
	outFile << "element face " << mdSize/3 << std::endl;
	
	outFile << "property list uchar int vertex_indices" << std::endl;
	outFile << "end_header" << std::endl;
	
	//output the vertex elements
	rtpMeshDataRemoveDuplicates(md);
	
	for(int i=0;i<mdSize;++i){
		outFile << md.x[i] << " ";
		outFile << md.y[i] << " ";
		outFile << md.z[i] << " ";
		
		if(md.nx.size())outFile << md.nx[i] << " ";
		if(md.ny.size())outFile << md.ny[i] << " ";
		if(md.nz.size())outFile << md.nz[i] << " ";
		
		if(md.u.size())outFile << md.u[i] << " ";
		if(md.v.size())outFile << md.v[i] << " ";
		
		if(md.r.size())outFile << md.r[i] << " ";
		if(md.g.size())outFile << md.g[i] << " ";
		if(md.b.size())outFile << md.b[i];
		
		outFile << std::endl;
	}
	
	for(int i=0;i<mdSize;i+=3){
		int f0, f1, f2;
		
		f0 = rtpFindFirstIndex(md, i);
		f1 = rtpFindFirstIndex(md, i+1);
		f2 = rtpFindFirstIndex(md, i+2);
		
		outFile << "3 " << f0 << " " << f1 << " " << f2 << std::endl;
	}
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