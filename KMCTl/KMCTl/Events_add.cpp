#include "Events.h"

void Events::addNewEvents(int src, int dest)
{
   if (src != c::invalidPosition) {
      if (grid.present(src) == atomType::nothing)
         addNewDestEvents(src);
      for (int i = 0; i < 6; i++) {
         int pom = neigh::neigh(i, src);
         if (grid.present(pom) == atomType::metal)
            addNewSrcEvents(pom);
         else if (grid.present(pom) == atomType::nothing)
            addNewDestEvents(pom);
      }
   }
   if (dest != c::invalidPosition) {
      if (grid.present(dest) == atomType::metal)
		   addNewSrcEvents(dest);
      for (int i = 0; i < 6; i++) {
         int pom = neigh::neigh(i, dest);
         if (grid.present(pom) == atomType::metal)
            addNewSrcEvents(pom);
         else if (grid.present(pom) == atomType::nothing)
            addNewDestEvents(pom);
		}
	}
}

void Events::addNewSrcEvents(int src)
{
	int rate;
	int srcn = src * c::aboveDiffDirections;
	int dest;
	int srcCount = grid._lattice[src].getVicinity();
	int direction;

   if (eventLists.desorbPointers[src].isValid())
      ;
   else
		eventLists.properDesEventAddition(srcCount, src);

	for (int adi = 0; adi < 6; adi++) {
		direction = adi;
      if (eventLists.srcPointers[srcn + direction].isValid())
         ;
      else {
			dest = neigh::neigh(adi, src);
			if (grid.present(dest) == atomType::nothing) {
				rate = c::getDiffType(srcCount, grid._lattice[dest].getVicinity() - 1, grid.getEdgeType(src, direction));
				eventLists.properDiffEventAddition(rate, SD(src, dest, direction));
			}
		}
	}
}

void Events::addNewDestEvents(int dest)
{

	int rate;
	int destn = dest * c::aboveDiffDirections;
	int src;
   int destCount = grid._lattice[dest].getVicinity();
	int direction;

	for (int adi = 0; adi < 6; adi++) {
		direction = (adi + 3) % 6;
      if (eventLists.destPointers[destn + direction].isValid())
         ;
      else {
			src = neigh::neigh(adi, dest);
         if (grid.present(src) == atomType::metal) {
				rate = c::getDiffType(grid._lattice[src].getVicinity(), destCount - 1, grid.getEdgeType(src, direction));
				eventLists.properDiffEventAddition(rate, SD(src, dest, direction));
			}
		}
	}
}