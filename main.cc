//main.cc

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>

#include "MeshFind.h"
#include "RawToPLY.h"

void printUsage(){
	std::cout << "Usage :" << std::endl;
	std::cout << "MeshFind [options]" << std::endl;
}

void printOptions(){
	std::cout << "Options :" << std::endl;
	std::cout << "  -s, --start    The begining of the mesh data within the file" << std::endl;
	std::cout << "  -e, --end      The end of the mesh data within the file" << std::endl;
	std::cout << "  -f, --format   How the mesh data is formated" << std::endl;
	std::cout << "  -a, --analyze  Analyze the file. Search for potential float arrays" << std::endl;
	std::cout << "  -o, --output   Output file name (*.ply)" << std::endl;
	std::cout << "  -i, --input    Binary input file name" << std::endl;
	std::cout << "  -h, --help     Print help information and exit" << std::endl;
}

int main(int argc, char **argv){
	std::ifstream inFile;
	
	int start = 0;
	int end = 0;
	std::string format = "";
	bool analyze = false;
	std::string outFileName;
	std::string inFileName;
	
	//go through arguments
	for(int i=1;i<argc;++i){
		if( !strcmp(argv[i],"-h") || !strcmp(argv[i],"--help")){
			printUsage();
			printOptions();
			exit(0);
		}
		
		if( !strcmp(argv[i],"-i") || !strcmp(argv[i],"--input")){
			i++;
			if(i<argc){
				inFileName = argv[i];
			} else {
				std::cout << "Couldn't get argument for input file!" << std::endl;
				exit(-1);
			}
		}
		
		if( !strcmp(argv[i],"-o") || !strcmp(argv[i],"--output")){
			i++;
			if(i<argc){
				outFileName = argv[i];
			} else {
				std::cout << "Couldn't get argument for output file!" << std::endl;
				exit(-1);
			}
		}
		
		if( !strcmp(argv[i],"-s") || !strcmp(argv[i],"--start")){
			i++;
			if(i<argc){
				start = atoi(argv[i]);
			} else {
				std::cout << "Couldn't get argument for start position" << std::endl;
				exit(-1);
			}
		}
		
		if( !strcmp(argv[i],"-e") || !strcmp(argv[i],"--end")){
			i++;
			if(i<argc){
				end = atoi(argv[i]);
			} else {
				std::cout << "Couldn't get argument for end position" << std::endl;
				exit(-1);
			}
		}
		
		if( !strcmp(argv[i],"-f") || !strcmp(argv[i],"--format")){
			i++;
			if(i<argc){
				format = argv[i];
			} else {
				std::cout << "Couldn't get argument for format" << std::endl;
				exit(-1);
			}
		}
		
		if( !strcmp(argv[i],"-a") || !strcmp(argv[i],"--analyze")){
			analyze = true;
		}
		
	}
	
	char *data;
	size_t dataSize;
	
	std::vector<mfMeshRange> mrv;
	mfEvalRules er;
	
	//initialize rules
	er.minLength = 100;
	er.min = -400.f;
	er.max = 400.f;
	
	if(inFileName.size()==0){
		std::cout << "No file selected!" << std::endl;
		exit(1);
	}
	
	inFile.open(inFileName, std::ios::binary);
	
	if(!inFile.is_open()){
		std::cout << inFileName << " couldn't be opened!" << std::endl;
		exit(1);
	}
	
	
	//get file size
	inFile.seekg(0, std::ios::end);
	dataSize = inFile.tellg();
	inFile.seekg(0, std::ios::beg);
	dataSize -= inFile.tellg();
	
	std::cout << "Reading " << inFileName << "..." << std::endl;
	
	data = new char[dataSize];
	inFile.read(data, dataSize);
	
	inFile.close();//no longer needed
	

	if(analyze){
		
		std::cout << "Searching for ranges of valid floating point numbers..." << std::endl;
		
		mfRangeFindFloat(mrv, &er, (uint8_t*)data, (uint8_t*)data+dataSize);
		
		std::cout << "Found potential ranges at : " << std::endl;
		for(int i=0; i<mrv.size(); ++i){
			int offset, stride;
			
			std::cout << " -range " << i << std::endl;
			std::cout << "   start : " << (char*)mrv[i].start-data << std::endl;
			std::cout << "   end   : " << (char*)mrv[i].end-data << std::endl;
			
			if( mfHasNormals(&offset, &stride, 
				mrv[i].start, mrv[i].end) ){
				//we have normals
				std::cout << "   *potential normals found at offset " << offset << " and stride " << stride << std::endl;
			} 
			
		}
	}
	
	if(start<end && !format.empty()){//output some data
		if(outFileName.empty()){
			outFileName = inFileName;
			
			//shave off any potential directory information
			outFileName.erase(outFileName.begin(), outFileName.begin() + outFileName.find_last_of('\\')+1);
			outFileName.erase(outFileName.begin(), outFileName.begin() + outFileName.find_last_of('/')+1);
			
			//erase extension, and add _extracted.ply
			outFileName.erase(outFileName.begin() + outFileName.find_last_of('.'), outFileName.end());
			outFileName += "_extracted.ply";
		}
		
		std::cout << "exporting file " << outFileName << std::endl;
		rtpExportRange(outFileName, format, (float*)&data[start], (float*)&data[end]);
		std::cout << "done!" << std::endl;
	}
	
	delete data;
	
	return 0;
}