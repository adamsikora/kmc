#include "Events.h"

void Events::adsorption(int position, atomType adatom)
{
   int dest = position;
   if (_getCoverage() < 1.0) {
      while (grid.present(dest) != atomType::nothing) {
         int smer = static_cast<int> (floor(_random() * 6));
         dest = neigh::neigh(smer, dest);
      }

      deleteObsoleteEvents(c::invalidPosition, dest);

      grid.updateLattice(Change(0, 0, c::invalidPosition), Change(atomType::nothing, adatom, dest));

      addNewEvents(c::invalidPosition, dest);

      _nAds++;
      if (adatom == atomType::metal)
         _nMetal++;
      else
         _nSilicon++;
   }
}

void Events::desorption(S desEvent)
{
   deleteObsoleteEvents(desEvent.src, c::invalidPosition);

   grid.updateLattice(Change(atomType::metal, atomType::nothing, desEvent.src), Change(0, 0, c::invalidPosition));

	addNewEvents(desEvent.src, c::invalidPosition);

   _nDes++;
	_nMetal--;
}

void Events::diffusion(SD diffEvent)
{
	deleteObsoleteEvents(diffEvent.getsrc(), diffEvent.getdest());

   grid.updateLattice(
      Change(atomType::metal, atomType::nothing, diffEvent.getsrc()),
      Change(atomType::nothing, atomType::metal, diffEvent.getdest())
		);

	addNewEvents(diffEvent.getsrc(), diffEvent.getdest());

   _nDiff++;
}