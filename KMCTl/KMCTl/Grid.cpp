#include "Grid.h"

void Grid::updateLattice(const Change& src, const Change& dest)  // update lattice state after event
{
	if (src.position != c::invalidPosition)
		_lattice[src.position].setAtom(src.actual);
   if (dest.position != c::invalidPosition)
		_lattice[dest.position].setAtom(dest.actual);

   if (src.position != c::invalidPosition) {
      int alter = c::atomBase[src.actual] - c::atomBase[src.old];
      for (int ui = 0; ui < 6; ui++) {
         _lattice[neigh::neigh(ui, src.position)].alterVicinity(alter);
      }
   }
   if (dest.position != c::invalidPosition) {
      int alter = c::atomBase[dest.actual] - c::atomBase[dest.old];
      for (int ui = 0; ui < 6; ui++) {
         _lattice[neigh::neigh(ui, dest.position)].alterVicinity(alter);
      }
   }
   if (src.position != c::invalidPosition) {
      _lattice[src.position].setVicinity(vicinityCount(src.position));
   }
   if (dest.position != c::invalidPosition) {
      _lattice[dest.position].setVicinity(vicinityCount(dest.position));
   }
}
