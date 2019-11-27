//main.cc

#include <iostream>
#include <fstream>
#include <vector>

#include "MeshFind.h"

int main(int argc, char **argv){
	std::ifstream infile;
	char *data;
	size_t dataSize;
	
	std::vector<mfMeshRange> mrv;
	mfEvalRules er;
	
	//initialize rules
	er.minLength = 100;
	er.min = -400.f;
	er.max = 400.f;
	
	if(argc <= 1){
		std::cout << "No file selected!" << std::endl;
		exit(1);
	}
	
	infile.open(argv[1], std::ios::binary);
	
	if(!infile.is_open()){
		std::cout << argv[1] << " couldn't be opened!" << std::endl;
		exit(1);
	}
	
	//get file size
	infile.seekg(0, std::ios::end);
	dataSize = infile.tellg();
	infile.seekg(0, std::ios::beg);
	dataSize -= infile.tellg();
	
	std::cout << "Reading " << argv[1] << "..." << std::endl;
	
	data = new char[dataSize];
	infile.read(data, dataSize);
	
	infile.close();//no longer needed
	
	std::cout << "Searching for ranges of valid floating point numbers..." << std::endl;
	
	mfRangeFindFloat(mrv, &er, (uint8_t*)data, (uint8_t*)data+dataSize);
	
	std::cout << "Found potential ranges at : " << std::endl;
	for(int i=0; i<mrv.size(); ++i){
		int offset, stride;
		std::cout << "\t" << (mrv[i].start-(float*)data) << " : " << (mrv[i].end-(float*)data);
		
		if( mfHasNormals(&offset, &stride, 
			mrv[i].start, mrv[i].end) ){
			//we have normals
			std::cout << " normals found at offset " << offset << " and stride " << stride;
		}
		
		std::cout << std::endl;
	}
	
	std::cout << "done!" << std::endl;
	delete data;
	
	return 0;
}