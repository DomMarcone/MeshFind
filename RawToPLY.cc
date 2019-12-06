//RawToPLY.cc

#include "RawToPLY.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
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


int rtpFormatElementCount(std::string::iterator fBegin, std::string::iterator fEnd){
	int result = 0;
	for(; fBegin < fEnd ;++fBegin){
		switch(*fBegin){
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
	int result = 0;
	
	if(md.x.size())result++;
	if(md.y.size())result++;
	if(md.z.size())result++;
	
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
	
	if(md.x[a] != md.x[b])return false;
	if(md.y[a] != md.y[b])return false;
	if(md.z[a] != md.z[b])return false;
	
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


bool rtpCompareMeshDataIndex2(MeshData &md0, MeshData &md1, int i0, int i1){
	int size0 = rtpMeshDataLength(md0);	
	int size1 = rtpMeshDataLength(md1);	
	if(i0<0 || i0>=size0)return false;
	if(i1<0 || i1>=size1)return false;
		
	if(md0.x[i0] != md1.x[i1])return false;
	if(md0.y[i0] != md1.y[i1])return false;
	if(md0.z[i0] != md1.z[i1])return false;
	
	if(md0.nx.size()>0 && md1.nx.size()>0 && md0.nx[i0] != md1.nx[i1])return false;
	if(md0.ny.size()>0 && md1.ny.size()>0 && md0.ny[i0] != md1.ny[i1])return false;
	if(md0.nz.size()>0 && md1.nz.size()>0 && md0.nz[i0] != md1.nz[i1])return false;
	
	if(md0.u.size()>0 && md1.u.size()>0 && md0.u[i0] != md1.u[i1])return false;
	if(md0.v.size()>0 && md1.v.size()>0 && md0.v[i0] != md1.v[i1])return false;
	
	if(md0.r.size()>0 && md1.r.size()>0 && md0.r[i0] != md1.r[i1])return false;
	if(md0.g.size()>0 && md1.g.size()>0 && md0.g[i0] != md1.g[i1])return false;
	if(md0.b.size()>0 && md1.b.size()>0 && md0.b[i0] != md1.b[i1])return false;
	
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

int rtpFindMatchingIndex(MeshData &md, MeshData &target, int index){
	if(index >= rtpMeshDataLength(md))return 0;

	for(int i=0;i<rtpMeshDataLength(target);++i){
		if(rtpCompareMeshDataIndex2(md,target,index,i)){
			return i;
		}
	}

	return 0;
}


//count the number of comma delimited sections
int rtpGetFormatSections(std::string format){
	//cache result.
	//todo : make this thread safe
	static int last_result = 0;
	static std::string *last_format = 0;

	if(last_format == &format){
		return last_result;
	} else {
		last_format = &format;
		last_result = std::count(format.begin(), format.end(), ',') + 1;
	}
	
	return last_result;
}


std::string::iterator rtpGetFormatSectionStart(std::string format, int section){
	if(section>=rtpGetFormatSections(format))return format.end();
	
	for(int i=0; i<format.size(); ++i){
		if(format[i]==',')section--;
		if(section<=0)return format.begin() + i;
	}
	//didn't find enough commas
	return format.end();
}


void rtpFillVector(std::vector<float> &fv, int offset, int stride, float *start, float *end){
	start += offset;
	for(;start<end;start+=stride)
		fv.push_back(*start);
}


void rtpFillMeshData(MeshData &md, std::string format, float *start, float *end){
	int sectionCount = rtpGetFormatSections(format);
	int totalElements = rtpFormatElementCount(format.begin(), format.end());
	int dataSize = (int)(end-start)/sizeof(float);
	int elementsSoFar = 0;
	
	for(int section=0; section<sectionCount; ++section){
		
		std::string::iterator sBegin = rtpGetFormatSectionStart(format, section);
		std::string::iterator sEnd = rtpGetFormatSectionStart(format, section+1);
		
		int stride = rtpFormatElementCount(sBegin, sEnd);
		int offset = 0;
		
		//todo : do testing here
		float *newStart = start + dataSize*elementsSoFar/totalElements;
		float *newEnd = start + dataSize*( elementsSoFar+stride )/totalElements;
	
		for(;sBegin < sEnd; ++sBegin){
				
			switch(*sBegin){
			case 'x' :
				rtpFillVector(md.x, offset++, stride, newStart, newEnd);
				break;
				
			case 'y' :
				rtpFillVector(md.y, offset++, stride, newStart, newEnd);
				break;
				
			case 'z' :
				rtpFillVector(md.z, offset++, stride, newStart, newEnd);
				break;

			case 'n' : //normals
				sBegin++;//what's the next character?
				switch(*sBegin){
				case 'x' :
					rtpFillVector(md.nx, offset++, stride, newStart, newEnd);
					break;
					
				case 'y' :
					rtpFillVector(md.ny, offset++, stride, newStart, newEnd);
					break;
					
				case 'z' :
					rtpFillVector(md.nz, offset++, stride, newStart, newEnd);
					break;
				}
				break;//END NORMALS
				
			case 'u' :
				rtpFillVector(md.u, offset++, stride, newStart, newEnd);
				break;
				
			case 'v' :
				rtpFillVector(md.v, offset++, stride, newStart, newEnd);
				break;
				
			case 'r' :
				rtpFillVector(md.r, offset++, stride, newStart, newEnd);
				break;
				
			case 'g' :
				rtpFillVector(md.g, offset++, stride, newStart, newEnd);
				break;
				
			case 'b' :
				rtpFillVector(md.b, offset++, stride, newStart, newEnd);
				break;
			
			case 'p' : //padding
				offset++;
				break;
			}
		}
		
		elementsSoFar += stride;
	}//end section loop
}

void rtpMeshDataCopy(MeshData &destination, MeshData &source){
	
	destination.x = source.x;
	destination.y = source.y;
	destination.z = source.z;
	destination.nx = source.nx;
	destination.ny = source.ny;
	destination.nz = source.nz;
	destination.u = source.u;
	destination.v = source.v;
	destination.r = source.r;
	destination.g = source.g;
	destination.b = source.b;
}

void rtpMeshDataRemoveDuplicates(MeshData &md){
	int meshLength = rtpMeshDataLength(md);
	
	for(int i=1;i<meshLength;){
		if(i != rtpFindFirstIndex(md,i)){//this index is a repeat
			//so remove it
			
			if(md.x.size()>i)md.x.erase(md.x.begin()+i);
			if(md.y.size()>i)md.y.erase(md.y.begin()+i);
			if(md.z.size()>i)md.z.erase(md.z.begin()+i);
			
			if(md.nx.size()>i)md.nx.erase(md.nx.begin()+i);
			if(md.ny.size()>i)md.ny.erase(md.ny.begin()+i);
			if(md.nz.size()>i)md.nz.erase(md.nz.begin()+i);
			
			if(md.u.size()>i)md.u.erase(md.u.begin()+i);
			if(md.v.size()>i)md.v.erase(md.v.begin()+i);
			
			if(md.r.size()>i)md.r.erase(md.r.begin()+i);
			if(md.g.size()>i)md.g.erase(md.g.begin()+i);
			if(md.b.size()>i)md.b.erase(md.b.begin()+i);
			
			//and account for the change in length
			meshLength = rtpMeshDataLength(md);
			
		} else {
			i++;
		}
	}
	
}

void rtpExportMeshData(MeshData &md, int faceVerts, std::string filename){
	std::ofstream outFile(filename);
	int mdCompactSize, mdSize = rtpMeshDataLength(md);
	MeshData mdCompact;
	
	if(!outFile.is_open()){
		std::cout << "Error creating " << filename << " for export!" << std::endl;
		exit(-1);
	}
	
	//copy and remove duplicates...
	rtpMeshDataCopy(mdCompact, md);
	rtpMeshDataRemoveDuplicates(mdCompact);
	mdCompactSize = rtpMeshDataLength(mdCompact);
	
	//Create the file header...
	outFile << "ply" << std::endl;
	outFile << "format ascii 1.0" << std::endl;
	outFile << "comment Generated with MeshFind" << std::endl;
	outFile << "element vertex " << mdCompactSize << std::endl;
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
	
	outFile << "element face " << mdSize/faceVerts << std::endl;
	
	outFile << "property list uchar int vertex_indices" << std::endl;
	outFile << "end_header" << std::endl;
	
	//output the vertex elements
	
	for(int i=0;i<mdCompactSize;++i){
		outFile << mdCompact.x[i] << " ";
		outFile << mdCompact.y[i] << " ";
		outFile << mdCompact.z[i] << " ";
		
		if(mdCompact.nx.size())outFile << mdCompact.nx[i] << " ";
		if(mdCompact.ny.size())outFile << mdCompact.ny[i] << " ";
		if(mdCompact.nz.size())outFile << mdCompact.nz[i] << " ";
		
		if(mdCompact.u.size())outFile << mdCompact.u[i] << " ";
		if(mdCompact.v.size())outFile << mdCompact.v[i] << " ";
		
		if(mdCompact.r.size())outFile << mdCompact.r[i] << " ";
		if(mdCompact.g.size())outFile << mdCompact.g[i] << " ";
		if(mdCompact.b.size())outFile << mdCompact.b[i];
		
		outFile << std::endl;
	}
	
	for(int i=0;i<mdSize;i+=faceVerts){
		outFile << faceVerts << " "; 
		
		for(int j=0;j<faceVerts;++j){
			outFile << rtpFindMatchingIndex(md, mdCompact, i+j) << " ";
		}
		outFile << std::endl;
	}
	outFile.close();
}


int rtpExportRange(std::string filename, std::string format, int faceVerts, float *start, float *end){
	MeshData md;
	//todo...
	rtpFillMeshData(md, format, start, end);

	rtpExportMeshData(md, faceVerts, filename);
	return 0;
}
