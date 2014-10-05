#include "Events.h"

void Events::deleteObsoleteEvents(int src, int dest)
{
	if (src != c::invalidPosition) {
		deleteObsoleteSrcEvents(src);
		for (int di = 0; di < 6; di++) {
			int pom = neigh::neigh(di, src);
         if (grid.present(pom) == atomType::metal)
            deleteObsoleteSrcEvents(pom);
         else if (grid.present(pom) == atomType::nothing)
            deleteObsoleteDestEvents(pom);
		}
	}
	if (dest != c::invalidPosition) {
		deleteObsoleteDestEvents(dest);
		for (int di = 0; di < 6; di++) {
			int pom = neigh::neigh(di, dest);
         if (grid.present(pom) == atomType::metal)
            deleteObsoleteSrcEvents(pom);
         else if (grid.present(pom) == atomType::nothing)
            deleteObsoleteDestEvents(pom);
		}
   }
}

void Events::deleteObsoleteSrcEvents(int position)
{
	if (eventLists.desorbPointers[position].isValid())
		eventLists.properDesEventReplace(eventLists.desorbPointers[position]); // del des
	for (int di = position * c::aboveDiffDirections; di < position * c::aboveDiffDirections + c::maxDiffDirections; di++)
		if (eventLists.srcPointers[di].isValid())
			eventLists.properDiffEventReplace(eventLists.srcPointers[di]); //del src diff
}

void Events::deleteObsoleteDestEvents(int position)
{
	for (int di = position * c::aboveDiffDirections; di < position * c::aboveDiffDirections + c::maxDiffDirections; di++)
		if (eventLists.destPointers[di].isValid())
			eventLists.properDiffEventReplace(eventLists.destPointers[di]); //del dest diff
}