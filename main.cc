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
	std::cout << "  -s, --start       The begining of the mesh data within the file" << std::endl;
	std::cout << "  -e, --end         The end of the mesh data within the file" << std::endl;
	std::cout << "  -f, --format      How the mesh data is formated" << std::endl;
	std::cout << "  -a, --analyze     Analyze the file" << std::endl;
	std::cout << "  -o, --output      Output file name (*.ply)" << std::endl;
	std::cout << "  -i, --input       Binary input file name" << std::endl;
	std::cout << "  --face-verts      Number of verticies per face" << std::endl;
	std::cout << "  -v, --version     Print version information" << std::endl;
	std::cout << "  -h, --help        Print help information and exit" << std::endl;
}

void printFormat(){
	std::cout << "Format Options :" << std::endl;
	std::cout << "  x y z             vertex position coordinates" << std::endl;
	std::cout << "  nx ny nz          normal directions" << std::endl;
	std::cout << "  u v               texture coordinates" << std::endl;
	std::cout << "  r g b             red green and blue vertex colors" << std::endl;
	std::cout << "  p                 padding, which doesn't contribute to the output" << std::endl;
	std::cout << "  ,                 indicate offset data (for non-interleaved floats)(*beta)" << std::endl;
}

void printVersion(){
	std::cout << "Compiled on " << __TIME__ << " " << __DATE__ << std::endl;
}

int main(int argc, char **argv){
	std::ifstream inFile;
	
	bool exitAfterArgs = false;
	
	int start = 0;
	int end = -1;
	std::string format = "";
	bool analyze = false;
	std::string outFileName;
	std::string inFileName;
	int faceVerts = 3;
	
	//go through arguments
	for(int i=1;i<argc;++i){
		//print help
		if( !strcmp(argv[i],"-h") || !strcmp(argv[i],"--help")){
			printUsage();
			std::cout << std::endl;
			printOptions();
			std::cout << std::endl;
			printFormat();
			std::cout << std::endl;
			exitAfterArgs = true;
		}
		
		//print version
		if( !strcmp(argv[i],"-v") || !strcmp(argv[i],"--version")){
			printVersion();
			std::cout << std::endl;
		}
		
		//input file name
		if( !strcmp(argv[i],"-i") || !strcmp(argv[i],"--input")){
			i++;
			if(i<argc){
				inFileName = argv[i];
			} else {
				std::cout << "Couldn't get argument for input file!" << std::endl;
				exit(-1);
			}
		}
		
		//output file name
		if( !strcmp(argv[i],"-o") || !strcmp(argv[i],"--output")){
			i++;
			if(i<argc){
				outFileName = argv[i];
			} else {
				std::cout << "Couldn't get argument for output file!" << std::endl;
				exit(-1);
			}
		}
		
		//start
		if( !strcmp(argv[i],"-s") || !strcmp(argv[i],"--start")){
			i++;
			if(i<argc){
				start = atoi(argv[i]);
			} else {
				std::cout << "Couldn't get argument for start position!" << std::endl;
				exit(-1);
			}
		}
		
		//end
		if( !strcmp(argv[i],"-e") || !strcmp(argv[i],"--end")){
			i++;
			if(i<argc){
				end = atoi(argv[i]);
			} else {
				std::cout << "Couldn't get argument for end position!" << std::endl;
				exit(-1);
			}
		}
		
		//format string
		if( !strcmp(argv[i],"-f") || !strcmp(argv[i],"--format")){
			i++;
			if(i<argc){
				format = argv[i];
			} else {
				std::cout << "Couldn't get argument for format!" << std::endl;
				exit(-1);
			}
		}
		
		//analyze?
		if( !strcmp(argv[i],"-a") || !strcmp(argv[i],"--analyze")){
			analyze = true;
		}
		
		//face verticies
		if( !strcmp(argv[i],"--face-verts")){
			i++;
			if(i<argc){
				faceVerts = atoi(argv[i]);
			} else {
				std::cout << "Couldn't get argument for the number of verticies per face!" << std::endl;
				exit(-1);
			}
		}
	}
	
	if(exitAfterArgs){
		exit(0);
	}
	
	char *data;
	size_t dataSize;
	
	
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
	
	if(end>dataSize){
		std::cout << "end is past the end of data!" << std::endl;
		end = dataSize;
	}	
	
	data = new char[end-start];
	inFile.seekg(start, std::ios::beg);
	inFile.read(data, end-start);
	
	inFile.close();//no longer needed
	

	if(analyze){
		std::vector<mfMeshRange> mrv;
		std::vector<mfIndexRange> irv;
		mfEvalRules er;
		
		//initialize rules
		er.minLength = 100;
		er.min = -800.f;
		er.max = 800.f;
		
		std::cout << "Searching for ranges of valid floating point numbers..." << std::endl;
		
		mfRangeFindFloat(mrv, &er, data, data+(end-start));
		
		mfRangeFindIndices(irv, er.minLength, data, data+(end-start));

		
		if(mrv.size()>0){
			std::cout << "Found potential float/vec3 array(s) at : " << std::endl;
			for(int i=0; i<mrv.size(); ++i){
				int offset, stride;
				mfRange range;
				
				mfGetRange(range, mrv[i].start, mrv[i].end);
				
				std::cout << " *float range " << i << std::endl;
				std::cout << "   start     : " << (char*)mrv[i].start-data + start << std::endl;
				std::cout << "   end       : " << (char*)mrv[i].end-data + start << std::endl;
				std::cout << "   length    : " << (int)(mrv[i].end - mrv[i].start)/sizeof(float) << std::endl;
				std::cout << "   min value : " << range.min << std::endl;
				std::cout << "   max value : " << range.max << std::endl;
				
				if( mfHasNormals(&offset, &stride, 
					mrv[i].start, mrv[i].end) ){
					//we have normals
					std::cout << "   potential normals found at offset " << offset << " and stride " << stride << std::endl;
				} 
				
				std::cout << std::endl;
			}
		} else {
			std::cout << "Found no potnential float arrays!" << std::endl;
		}
		
		if(irv.size()>0){
			std::cout << "Found potential index array(s) at :" << std::endl;
			
			for(int i=0; i<irv.size(); ++i){
				std::cout << " *integer range " << i << std::endl;
				std::cout << "   start  : " << (char*)irv[i].start-data + start << std::endl;
				std::cout << "   end    : " << (char*)irv[i].end-data + start << std::endl;
				std::cout << "   length : " << (int)(irv[i].end - irv[i].start)/sizeof(int) << std::endl;
			}
			
			std::cout << std::endl;
			
		} else {
			std::cout << "Found no potential index arrays!" << std::endl;
		}
	}
	
	//output some data
	if(start<end && !format.empty()){
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
		rtpExportRange(outFileName, format, faceVerts, (float*)data, (float*)&data[end-start]);
		std::cout << "done!" << std::endl;
	}
	
	delete data;
	
	return 0;
}