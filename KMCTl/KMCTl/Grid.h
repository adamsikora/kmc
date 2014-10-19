/*
author: Adam Sikora

defines class for managing lattice state

*/

#ifndef GRID_H
#define GRID_H

#include "Includes.h"
#include "Class.h"
#include "Neighborhood.h"

class Events;
class EventLists;

class GridCell
{
public:
   GridCell() : _atom(0), _vicinity(0) {}
   inline int getAtom() const { return _atom; };
   inline void setAtom(int atom) { _atom = atom; };
   inline int getVicinity() const { return _vicinity; };
   inline void setVicinity(int nC) { _vicinity = nC; };
   inline void alterVicinity(int nC) { _vicinity += nC; };

private:
   int _atom;
   int _vicinity;
};

class Grid
{
public:
	Grid(int initialHeight) : _lattice(c::A, GridCell()) {}

	friend class Events;
	friend class EventLists;

   void updateLattice(const Change& src, const Change& dest);    // updates lattice state after event

	inline int present(int pos) const { return _lattice[pos].getAtom();}
   // checks whether atom is present at given position
	
	inline int vicinityCount(int pos) { // counts neighboring atoms
      return
         c::atomBase[_lattice[neigh::n_0(pos)].getAtom()] +
         c::atomBase[_lattice[neigh::n_1(pos)].getAtom()] +
         c::atomBase[_lattice[neigh::n_2(pos)].getAtom()] +
         c::atomBase[_lattice[neigh::n_3(pos)].getAtom()] +
         c::atomBase[_lattice[neigh::n_4(pos)].getAtom()] +
         c::atomBase[_lattice[neigh::n_5(pos)].getAtom()];
	}

	inline int getEdgeType(int src, int direction) {
		int toRet = 0;
		if (_lattice[neigh::neigh((direction + 1) % 6, src)].getAtom() != atomType::nothing) {
			toRet += (direction % 2 == 0) ? 2 : 1;
		}
		if (_lattice[neigh::neigh((direction + 5) % 6, src)].getAtom() != atomType::nothing) {
			toRet += (direction % 2 == 0) ? 1 : 2;
		}
		return toRet;
	}

private:
	std::vector<GridCell> _lattice;  // actual lattice represented by 1D vector, helical boundary conditions used

};

#endif //GRID_H