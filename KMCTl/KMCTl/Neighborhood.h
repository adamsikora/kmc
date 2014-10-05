/*
author: Adam Sikora

defines functions for finding neighboring positions on lattice

header only

	neigh - nearest neighbors on same height		0 1
															  5 x 2
																4 3

*/

#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include "Includes.h"

namespace neigh
{
	using namespace c;

	const int neighShifts[6] = { A - w     , A - w + 1 , 1     , w , w - 1 , A - 1 };	
	
	inline int n_0(int p)  { return (p - w + A) % A;		}
	inline int n_1(int p)  { return (p + 1 - w + A) % A;	}
	inline int n_2(int p)  { return (p + 1) % A;				}
	inline int n_3(int p)  { return (p + w) % A;				}
	inline int n_4(int p)  { return (p - 1 + w) % A;		}
	inline int n_5(int p)  { return (p - 1 + A) % A;		}
	
	inline int neigh(int i, int p) { return (p + neighShifts[i]) % A; }

}


#endif // NEIGHBORHOOD_H