//RawToPLY.h
#pragma once
#ifndef _RAW_TO_PLY_H
#define _RAW_TO_PLY_H

/*
**  rtpExportRange() exports an array of raw floats to a 3d .ply file
**  -filename - the name of the file to be exported
**  -format - set what each float represents, whether it's the vertex location, 
**  	normal, texture coordinate, uv, color, or non-relevant data.
**  -start - a pointer to the initial float
**  -end - a pointer to the final float
**  
**  Format Arguments - format is a character string, which describes data between
**  	start and end.
**  -x, y, z - the positional cordinates of the vertex
**  -nx, ny, nz - the noraml direction of the coordinates
**  -u, v - texture coordinates
**  -r, g, b - color information
**  -p - padding, or data which is irrelevant that's the same as a float.
*/

#include <iostream>
#include <fstream>
#include <string>

int rtpExportRange(std::string filename, std::string format, int faceVerts, float *start, float *end);

#endif //_RAW_TO_PLY_H
