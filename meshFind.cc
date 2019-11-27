//a simple tool for finding, analyzing, and extracting potentially mesh-like float arrays
/*
**  Assumptions :
**  1. Mesh data should consist of reasonable integer or floating point values.
**  2. These values will exist within a practical range
**  3. There will be some common coordinates which can be used to deterimine stride and offset
*/

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

//How close to 1 must a vec3 length be to be considered a normal
#define NORMAL_TOLERANCE 0.001f 

typedef struct {
	int minLength;
	float min;
	float max;
} mfEvalRules;

typedef struct {
	float *start;
	float *end;
} mfMeshRange;

//determine range
bool evalFloat(float f, EvalRules *er){
	if(f < er->min)return false;
	if(f > er->max)return false;
	//put other constraints here
	return true;
}

//report the end of an array of vertex-like floats
float *mfGetMeshEndFloat(EvalRules *er, float *start, float *cutoff){
	for(;start<cutoff && evalFloat(*start,er);start++);
	return --start;//go back, before the invalid float
}

void mfRangeFindFloat(std::vector<MeshRange> &fv, EvalRules *er, uint8_t *start, uint8_t *end){
	float *newEnd = (float*)end-(sizeof(float)*er->minLength);
	float *endF = (float*)end;
	
	for(;start<end;start++){
		float *startF = (float*)start;
		if(evalFloat(*startF,er)){
			float *meshEnd = getMeshEndFloat(er,startF,endF);
			
			if( (meshEnd-startF)/sizeof(float) >= er->minLength ){
				//we have found a valid range!
				MeshRange mr;
				mr.start = startF;
				mr.end = meshEnd;
				fv.push_back(mr);
				
				start = (uint8_t*)meshEnd;
			}
		}
	}
}

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
		std::cout << "\t" << (mrv[i].start-(float*)data) << " : " << (mrv[i].end-(float*)data) << std::endl;
	}
	
	std::cout << "done!" << std::endl;
	delete data;
	
	return 0;
}
