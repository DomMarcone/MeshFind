//MeshData.cc

#include "MeshData.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

int mdFormatElementCount(std::string::iterator fBegin, std::string::iterator fEnd){
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


int mdMeshDataElementCount(MeshData &md){
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

int mdMeshDataLength(MeshData &md){
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


bool mdCompareMeshDataIndex(MeshData &md, int a, int b){
	int size = mdMeshDataLength(md);	
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


bool mdCompareMeshDataIndex2(MeshData &md0, MeshData &md1, int i0, int i1){
	int size0 = mdMeshDataLength(md0);	
	int size1 = mdMeshDataLength(md1);	
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


int mdFindFirstIndex(MeshData &md, int index){
	if(index >= mdMeshDataLength(md))return 0;

	for(int i=0;i<index;++i){
		if(mdCompareMeshDataIndex(md,index,i)){
			return i;
		}
	}

	return index;
}

int mdFindMatchingIndex(MeshData &md, MeshData &target, int index){
	if(index >= mdMeshDataLength(md))return 0;

	for(int i=0;i<mdMeshDataLength(target);++i){
		if(mdCompareMeshDataIndex2(md,target,index,i)){
			return i;
		}
	}

	return 0;
}


//count the number of comma delimited sections
int mdGetFormatSections(std::string format){
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

//check the format string for validity
bool mdValidFormat(std::string format){
	char count1[] = {'v', 'u', 'r', 'g', 'b'};
	char count2[] = {'x', 'y', 'z'};
	char count3[] = {'n'};
	
	for(auto i : count1){
		if(std::count(format.begin(), format.end(), i) > 1)return false;
	}
	
	for(auto i : count2){
		if(std::count(format.begin(), format.end(), i) > 2)return false;
	}
	
	for(auto i : count3){
		if(std::count(format.begin(), format.end(), i) > 3)return false;
	}
	
	//todo - further checks
	
	return true;
}


std::string::iterator mdGetFormatSectionStart(std::string format, int section){
	if(section>=mdGetFormatSections(format))return format.end();
	
	for(int i=0; i<format.size(); ++i){
		if(format[i]==',')section--;
		if(section<=0)return format.begin() + i;
	}
	//didn't find enough commas
	return format.end();
}


void mdFillVector(std::vector<float> &fv, int offset, int stride, float *start, float *end){
	start += offset;
	for(;start<end;start+=stride)
		fv.push_back(*start);
}


void mdFillMeshData(MeshData &md, std::string format, float *start, float *end){
	int sectionCount = mdGetFormatSections(format);
	int totalElements = mdFormatElementCount(format.begin(), format.end());
	int dataSize = (int)(end-start)/sizeof(float);
	int elementsSoFar = 0;
	
	for(int section=0; section<sectionCount; ++section){
		
		std::string::iterator sBegin = mdGetFormatSectionStart(format, section);
		std::string::iterator sEnd = mdGetFormatSectionStart(format, section+1);
		
		int stride = mdFormatElementCount(sBegin, sEnd);
		int offset = 0;
		
		//todo : do testing here
		float *newStart = start + dataSize*elementsSoFar/totalElements;
		float *newEnd = start + dataSize*( elementsSoFar+stride )/totalElements;
	
		for(;sBegin < sEnd; ++sBegin){
				
			switch(*sBegin){
			case 'x' :
				mdFillVector(md.x, offset++, stride, newStart, newEnd);
				break;
				
			case 'y' :
				mdFillVector(md.y, offset++, stride, newStart, newEnd);
				break;
				
			case 'z' :
				mdFillVector(md.z, offset++, stride, newStart, newEnd);
				break;

			case 'n' : //normals
				sBegin++;//what's the next character?
				switch(*sBegin){
				case 'x' :
					mdFillVector(md.nx, offset++, stride, newStart, newEnd);
					break;
					
				case 'y' :
					mdFillVector(md.ny, offset++, stride, newStart, newEnd);
					break;
					
				case 'z' :
					mdFillVector(md.nz, offset++, stride, newStart, newEnd);
					break;
				}
				break;//END NORMALS
				
			case 'u' :
				mdFillVector(md.u, offset++, stride, newStart, newEnd);
				break;
				
			case 'v' :
				mdFillVector(md.v, offset++, stride, newStart, newEnd);
				break;
				
			case 'r' :
				mdFillVector(md.r, offset++, stride, newStart, newEnd);
				break;
				
			case 'g' :
				mdFillVector(md.g, offset++, stride, newStart, newEnd);
				break;
				
			case 'b' :
				mdFillVector(md.b, offset++, stride, newStart, newEnd);
				break;
			
			case 'p' : //padding
				offset++;
				break;
			}
		}
		
		elementsSoFar += stride;
	}//end section loop
}

void mdMeshDataCopy(MeshData &destination, MeshData &source){
	
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

void mdMeshDataRemoveDuplicates(MeshData &md){
	int meshLength = mdMeshDataLength(md);
	
	for(int i=1;i<meshLength;){
		if(i != mdFindFirstIndex(md,i)){//this index is a repeat
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
			meshLength = mdMeshDataLength(md);
			
		} else {
			i++;
		}
	}
	
}