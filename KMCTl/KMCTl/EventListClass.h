/*
author: Adam Sikora

defines base class for event lists

just header

*/

#ifndef EVENTLISTCLASS_H
#define EVENTLISTCLASS_H

#include "Consts.h"

class S
{
public:
	S(int src) : src(src) {}

	int src;

};

class SD
{
public:
	SD(int src, int dest, int dir) :
      srcnDir(src * c::aboveDiffDirections + dir),
      destnDir(dest * c::aboveDiffDirections + dir) {}

	inline int getsrc() const { return srcnDir / c::aboveDiffDirections; }
	inline int getdest() const { return destnDir / c::aboveDiffDirections; }
	inline int getdir() const { return srcnDir % c::aboveDiffDirections; }

	int srcnDir, destnDir;

};

#endif //EVENTLISTCLASS_H