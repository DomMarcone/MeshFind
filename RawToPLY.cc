//RawToPLY.cc

#include "RawToPLY.h"
#include "MeshData.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


void rtpExportMeshData(MeshData &md, int faceVerts, std::string filename){
	std::ofstream outFile(filename);
	int mdCompactSize, mdSize = mdMeshDataLength(md);
	MeshData mdCompact;
	
	if(!outFile.is_open()){
		std::cout << "Error creating " << filename << " for export!" << std::endl;
		exit(-1);
	}
	
	//copy and remove duplicates...
	mdMeshDataCopy(mdCompact, md);
	mdMeshDataRemoveDuplicates(mdCompact);
	mdCompactSize = mdMeshDataLength(mdCompact);
	
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
			outFile << mdFindMatchingIndex(md, mdCompact, i+j) << " ";
		}
		outFile << std::endl;
	}
	outFile.close();
}


int rtpExportRange(std::string filename, std::string format, int faceVerts, float *start, float *end){
	MeshData md;
	
	if(!mdValidFormat(format)){
		std::cout << "Invalid format string!" << std::endl;
		return 1;
	}
	
	mdFillMeshData(md, format, start, end);

	rtpExportMeshData(md, faceVerts, filename);
	return 0;
}
