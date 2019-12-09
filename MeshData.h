//MeshData.h
#pragma once
#ifndef _MESH_DATA_H
#define _MESH_DATA_H

#include <iostream>
#include <vector>

typedef struct {
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> z;
	std::vector<float> nx;
	std::vector<float> ny;
	std::vector<float> nz;
	std::vector<float> u;
	std::vector<float> v;
	std::vector<float> r;
	std::vector<float> g;
	std::vector<float> b;
} MeshData;

int mdMeshDataLength(MeshData &md);
void mdMeshDataCopy(MeshData &destination, MeshData &source);
int mdFindMatchingIndex(MeshData &md, MeshData &target, int index);
void mdFillMeshData(MeshData &md, std::string format, float *start, float *end);
void mdMeshDataRemoveDuplicates(MeshData &md);

bool mdValidFormat(std::string format);

#endif //_MESH_DATA_H
