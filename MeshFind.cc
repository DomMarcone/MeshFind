//MeshFind.cc

#include "MeshFind.h"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <cfloat>

bool mfEvalFloat(float f, mfEvalRules *er){
	if(std::isnan(f))return false;
	if(std::isinf(f))return false;
	if(f < er->min)return false;
	if(f > er->max)return false;
	//put other constraints here
	return true;
}


float *mfGetMeshEndFloat(mfEvalRules *er, float *start, float *cutoff){
	for(;start<cutoff && mfEvalFloat(*start,er);start++);
	return --start;//go back, before the invalid float
}


void mfRangeFindFloat(std::vector<mfMeshRange> &fv, mfEvalRules *er, char *start, char *end){
	char *newEnd = end-(sizeof(float)*er->minLength);
	float *endF = (float*)end;
	
	for(;start<newEnd; start++){
		float *startF = (float*)start;
		if(mfEvalFloat(*startF,er)){
			float *meshEnd = mfGetMeshEndFloat(er,startF,endF);
			
			if( (meshEnd-startF)/sizeof(float) >= er->minLength ){
				//we have found a valid range!
				mfMeshRange mr;
				mr.start = startF;
				mr.end = meshEnd;
				fv.push_back(mr);
				
				start = (char*)meshEnd;
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

//get the last 
int *mfGetIndiciesEnd(int maxValue, int *start, int *end){
	for(;start<end && 
		*start < maxValue &&
		*start >= 0;++start)
	return start--;
}


void mfRangeFindIndices(std::vector<mfIndexRange> &iv, int minLength, char *start, char *end){
	char *newEnd = end-(sizeof(int)*minLength);
	int *endI = (int*)end;
	int maxValue = ((int)start-(int)end)/sizeof(int);
	
	for(;start<newEnd; start++){
		int *startI = (int*)start;
		if(*startI >=0 && *startI <= maxValue){
			int *indexEnd = mfGetIndiciesEnd(maxValue, startI, endI);
			
			if( (indexEnd-startI)/sizeof(int) >= minLength ){
				//we have found a valid range!
				mfIndexRange ir;
				ir.start = startI;
				ir.end = indexEnd;
				iv.push_back(ir);
				
				start = (char*)indexEnd;
			}
		}
	}
}

