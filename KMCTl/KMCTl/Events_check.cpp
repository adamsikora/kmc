#include "Events.h"

int Events::crossCheck(bool showResults)
{
	bool badDes = false, badSrcDiff = false, badDestDiff = false;
	for (int ci = 0; ci < c::nDesTypes; ci++)			// des pointers check
		for (int cj = 0; cj < static_cast<int> (eventLists._desEvents[ci].size()); cj++) {
			PointerToEvent pomPointer = eventLists.desorbPointers[eventLists._desEvents[ci][cj].src];
			if (pomPointer.getPosinVector() != cj || pomPointer.getRate() != ci)
				badDes = true;
		}
	for (int ci = 0; ci < c::nDiffTypes; ci++) {
		for (int cj = 0; cj < static_cast<int> (eventLists._diffEvents[ci].size()); cj++) {
			PointerToEvent pomPointer =					// src diff pointers check
				eventLists.srcPointers[eventLists._diffEvents[ci][cj].srcnDir];
         if (pomPointer.getPosinVector() != cj || pomPointer.getRate() != ci)
				badSrcDiff = true;
			pomPointer =										// dest diff pointers check
				eventLists.destPointers[eventLists._diffEvents[ci][cj].destnDir];
         if (pomPointer.getPosinVector() != cj || pomPointer.getRate() != ci)
				badDestDiff = true;
		}
	}

	std::vector<int> desPointers(c::w * c::h, -1);
	std::vector<int> diffPointers[c::maxDiffDirections];
	for (int ci = 0; ci < c::maxDiffDirections; ci++)
		diffPointers[ci].assign(c::w * c::h, -1);
	
	for (int ci = 0; ci < c::h; ci++) {
		for (int cj = 0; cj < c::w; cj++) {
			if (grid._lattice[cj + ci * c::w].getAtom() == atomType::metal) {
				int pomSrc(cj + ci * c::w);
				int srcCount = grid.vicinityCount(pomSrc);
				desPointers[ci*c::w + cj] = srcCount;

				for (int adi = 0; adi < 6; adi++) {
					int direction = adi;
					int pomDest = neigh::neigh(adi, pomSrc);
					if (grid.present(pomDest) == atomType::nothing)
						diffPointers[direction][ci*c::w + cj] = c::getDiffType(srcCount, grid.vicinityCount(pomDest) - 1, grid.getEdgeType(pomSrc, direction));
				}
			}
		}
	}

	bool badDesorb = false, badDiff = false;

	for (int ci = 0; ci < c::w * c::h; ci++) {
		if (desPointers[ci] != eventLists.desorbPointers[ci].getRate())
			badDesorb = true;
		for (int cj = 0; cj < c::maxDiffDirections; cj++)
			if (diffPointers[cj][ci] != eventLists.srcPointers[ci * c::aboveDiffDirections + cj].getRate())
				badDiff = true;
	}

	int badVicinity = 0;
	for (int ci = 0; ci < c::h; ci++)
   for (int cj = 0; cj < c::w; cj++) {
      int pom(cj + ci * c::w);
      if (grid.vicinityCount(pom) != grid._lattice[cj + ci * c::w].getVicinity())
         badVicinity++;
   }

	if (showResults) {
		std::cout << "Integrity check:" << std::endl;

		if (badDes)
			std::cout << "\tspatne des pointery" << std::endl;
		else
			std::cout << "\tdes pointery       ok" << std::endl;
		if (badSrcDiff)
			std::cout << "\tspatne srcDiff pointery" << std::endl;
		else
			std::cout << "\tsrdDiff pointery   ok" << std::endl;
		if (badDestDiff)
			std::cout << "\tspatne destDiff pointery" << std::endl;
		else
			std::cout << "\tdestDiff pointery  ok" << std::endl;
		if (badDesorb)
			std::cout << "\tspatne des eventy" << std::endl;
		else
			std::cout << "\tdes eventy         ok" << std::endl;
		if (badDiff)
			std::cout << "\tspatne diff eventy" << std::endl;
		else
			std::cout << "\tdiff eventy        ok" << std::endl;
		if (badVicinity)
			std::cout << "\tnumber of bad vicinities:  " << badVicinity << std::endl;
		else
			std::cout << "\tvicinities         ok" << std::endl;
	}

	return badDes + badDesorb + badDestDiff + badDiff + badVicinity + badSrcDiff;
}

bool Events::checkDiff(int supposedConf, SD conf)
{
	if (supposedConf != c::getDiffType(grid._lattice[conf.getsrc()].getVicinity(), grid._lattice[conf.getdest()].getVicinity() - 1, grid.getEdgeType(conf.getsrc(), conf.getdir()))) {
		std::cout << supposedConf << " " << c::getDiffType(grid._lattice[conf.getsrc()].getVicinity(), grid._lattice[conf.getdest()].getVicinity() - 1, grid.getEdgeType(conf.getsrc(), conf.getdir()));
		std::cin.ignore();
		return false;
	}
	else {
		//std::cout << "dobre";
		//std::cin.ignore();
		return true;
	}
}