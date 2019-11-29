//MeshFind.cc

#include "MeshFind.h"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

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


float mfGetVec3Length(float *f){
	return (float)sqrt(
		f[0]*f[0] +
		f[1]*f[1] +
		f[2]*f[2]
	);
}


float mfGetRangeVec3AverageLength(int offset, int stride, float *start, float *end){
	float result=0.f;
	int count=0;
	
	start += offset;
	
	for(; start<(end-3); start+=stride){
		result += mfGetVec3Length(start);
		count++;
	}
	if(count==0)count=1;//avoid division by 0
	return result/count;
}

bool mfHasNormals(int *offset, int *stride, float *start, float *end){
	float closestDistance = NORMAL_TOLERANCE;
	*offset = -1;
	*stride = -1;
	bool result = false;
	
	for(int s=NORMAL_MIN_STRIDE; s<=NORMAL_MAX_STRIDE; s++){
		for(int o=0; o<s; o++){
			float v = mfGetRangeVec3AverageLength(o, s, start, end);
			
			//v should = 1 if the range is only valid normals
			v = fabs(v - 1.0);
			if( v <= closestDistance ){
				*offset = o;
				*stride = s;
				result = true;
			}
			
		}
	}
	
	return result;
}

