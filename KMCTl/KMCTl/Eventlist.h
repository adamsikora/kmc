/*
author: Adam Sikora

defines class for handling event lists

*/

#ifndef EVENTLIST_H
#define EVENTLIST_H

#include "Includes.h"
#include "Grid.h"

class Events;

class EventLists
{
public:
   EventLists(std::vector<GridCell>& reftoLattice) : lattice(reftoLattice),
      srcPointers(c::A * c::aboveDiffDirections, PointerToEvent(c::invalidPointer)),
      destPointers(c::A * c::aboveDiffDirections, PointerToEvent(c::invalidPointer)),
      desorbPointers(c::A, PointerToEvent(c::invalidPointer)) {}

	friend class Events;

   void properDesEventReplace(PointerToEvent ptrToEvent);     // replaces outdated desorption event
   void properDiffEventReplace(PointerToEvent ptrToEvent);    // replaces outdated diffusion event

	void properDesEventAddition(int rate, int source);    // adds new desorption event
	void properDiffEventAddition(int rate, SD sourcenDest);// adds new diffusion event

private:
	std::vector<GridCell>& lattice;

	std::vector<S> _desEvents[c::nDesTypes];
	std::vector<SD> _diffEvents[c::nDiffTypes];

	std::vector<PointerToEvent> srcPointers, destPointers, desorbPointers;

};

#endif //EVENTLIST_H