//MeshFind.cc

#include "MeshFind.h"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

//How close to 1 must a vec3 length be to be considered a normal
#define NORMAL_TOLERANCE 0.001f 

/* //defined in MeshFind.h
typedef struct {
	int minLength;
	float min;
	float max;
} mfEvalRules;

typedef struct {
	float *start;
	float *end;
} mfMeshRange;
*/


bool mfEvalFloat(float f, mfEvalRules *er){
	if(f < er->min)return false;
	if(f > er->max)return false;
	//put other constraints here
	return true;
}


float *mfGetMeshEndFloat(mfEvalRules *er, float *start, float *cutoff){
	for(;start<cutoff && mfEvalFloat(*start,er);start++);
	return --start;//go back, before the invalid float
}


void mfRangeFindFloat(std::vector<mfMeshRange> &fv, mfEvalRules *er, uint8_t *start, uint8_t *end){
	float *newEnd = (float*)end-(sizeof(float)*er->minLength);
	float *endF = (float*)end;
	
	for(;start<end;start++){
		float *startF = (float*)start;
		if(mfEvalFloat(*startF,er)){
			float *meshEnd = mfGetMeshEndFloat(er,startF,endF);
			
			if( (meshEnd-startF)/sizeof(float) >= er->minLength ){
				//we have found a valid range!
				mfMeshRange mr;
				mr.start = startF;
				mr.end = meshEnd;
				fv.push_back(mr);
				
				start = (uint8_t*)meshEnd;
			}
		}
	}
}

