//MeshFind.h
#pragma once
#ifndef _MESHFIND_H
#define _MESHFIND_H
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
bool mfEvalFloat(float f, mfEvalRules *er);

//report the end of an array of vertex-like floats
float *mfGetMeshEndFloat(mfEvalRules *er, float *start, float *cutoff);

void mfRangeFindFloat(std::vector<mfMeshRange> &fv, mfEvalRules *er, uint8_t *start, uint8_t *end);

#endif //_MESHFIND_H
