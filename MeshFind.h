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
#define NORMAL_TOLERANCE 0.01f //1%
#define NORMAL_MIN_STRIDE 6 //any less and it won't have verticies
#define NORMAL_MAX_STRIDE 12 //if we have 3 verts, 2 uvs, 3 norms, 3 colors, and 1 extra float for whatever


typedef struct {
	int minLength;
	float min;
	float max;
} mfEvalRules;

typedef struct {
	float *start;
	float *end;
} mfMeshRange;

typedef struct {
	int *start;
	int *end;
} mfIndexRange;

typedef struct {
	float min;
	float max;
} mfRange;

//determine range
bool mfEvalFloat(float f, mfEvalRules *er);

float mfGetVec3Length(float *f);

//report the end of an array of vertex-like floats
float *mfGetMeshEndFloat(mfEvalRules *er, float *start, float *cutoff);

void mfRangeFindFloat(std::vector<mfMeshRange> &fv, mfEvalRules *er, char *start, char *end);

void mfRangeFindIndices(std::vector<mfIndexRange> &iv, int minLength, char *start, char *end);

//stride and offset will pass back the stride and offset if ones are found.
bool mfHasNormals(int *offset, int *stride, float *start, float *end);

void mfGetRange(mfRange &r, float *start, float *end);

#endif //_MESHFIND_H
