/*
author: Adam Sikora
	
defines compilation time known constants and basic functions

just header

*/

#ifndef CONSTS_H
#define CONSTS_H

enum atomType {
   nothing, metal, silicon
};

namespace c
{
   const int maxDiffDirections = 6;                           // number of diffusion directions (6-nn)
   const int aboveDiffDirections = 8;                         // closest power of two for easy compression

   const int nNeighCount = 7;
   const int nAdatomTypes = 2;

   const int nVicinityTypes = nNeighCount * nNeighCount;
   const int nDesTypes = nVicinityTypes;

   const int nDiffChanges = 7;
	const int nDiffEdges = 4;
   const int nDiffTypes = nDiffChanges * nDiffChanges * nDiffEdges;

   const int atomBase[3] = { 0, 1, nNeighCount };

   const double boltzmann = 8.6173324e-5;

	const int w = 64;// 256;                                   // lattice size, should be power of two for faster computation
   const int h = w;
   const int A = w * h;


   const int invalidAtom = -1;
   const int invalidPointer = -1;
   const int invalidPosition = -1;

   inline int getDiffType(int srcVicinity, int destVicinity, int edgeType)    // returns integer with encoded diffusion type
   {
		return edgeType * c::nDiffChanges * c::nDiffChanges + (destVicinity / c::nNeighCount - srcVicinity / c::nNeighCount + 3) * c::nDiffChanges +
         destVicinity % c::nNeighCount - srcVicinity % c::nNeighCount + 3;
   }
}

#endif // CONSTS_H