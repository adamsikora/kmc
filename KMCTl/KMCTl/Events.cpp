#include "Events.h"

Events::Events(Report& report)
: para(report.parameters), results(report.results), evolution(para), rates(evolution._rates),
grid(para.initialHeight), eventLists(grid._lattice), _nDesorbNeigh(c::nNeighCount, 0)
{
	_nEvents = 0;
	_nAds = 0; _nDes = 0; _nDiff = 0; //_nDownDiff = 0; _nUpDiff = 0; _nFreeDes = 0;
	_nMetal = _nSilicon = 0;

	//_adsHeight = para.initialHeight + 1;

	_targetCoverage = para.targetCoverage;

	_random.seed(para.seed);

	/*if (para.initialHeight)
		for (int i = 0; i < c::A; i++)
			addNewSrcEvents(Position(i, 1));*/
}

//simulationReturn Events::desorptionSimulation()
//{
//   for (int i = 0; i < c::A / c::w; i++) {
//      executeDes();
//   }
//   unsigned __int64 atomsStep = c::A / 16;
//   unsigned __int64 targetAtoms = c::A - atomsStep;
//
//   while (_getCoverage() > _targetCoverage) {
//      if (_nEvents == 10000000 && _getCoverage() > 0.9) {
//         //return simulationReturn::excessiveDiffusion;
//      }
//      if (results.timeToAdd(_getCoverage()))
//         results.addRow(getResultVector());
//      if (_nEvents % 1000000 == 0) {
//         std::cout << _nEvents / 1000000 << "M\t" << _getCoverage() << std::endl;
//         //crossCheck();
//      }
//      if (_nAtom <= targetAtoms) {
//         targetAtoms -= atomsStep;
//         results.addGridState(grid._lattice);
//      }
//      execute();
//   }
//   if (_nDiff < _nDes *10) {
//      //return simulationReturn::negligibleDiffusion;
//   }
//
//   results.addDCD(evolution.theta.getRawDeltaCoverageDependency(), evolution.theta.getFineDeltaCoverageDependency());
//   results.addRow(getResultVector());
//
//   return simulationReturn::allright;
//}

void Events::adsorbIslands(int nIslands, int islandSize)
{
	for (int i = 0; i < nIslands; i++) {
		int position = static_cast<int> (floor(_random() * c::A));
		for (int j = 0; j < islandSize; j++) {
			adsorption(neigh::neigh(n, position), atomType::silicon);
		}
	}
}

simulationReturn Events::growthSimulation()
{
	/*std::cout << std::endl;
	for (int i = 0; i < 4; i++) {
		std::cout << evolution._rates.getRate(0, i, 0);
	}
	for (int i = 0; i < 4; i++) {
		std::cout << evolution._rates.getRate(i, 0, 0);
	}
	std::cin.ignore();*/

	int position = c::A / 2 + c::w / 3;
	//for (int n = 0; n < 6; n++) {
	//	adsorption(neigh::neigh(n,position), atomType::silicon);
	//}
	adsorption(position, atomType::silicon);

	int nShows = 10;
	double currTarget = _targetCoverage / 10;

   while (_getCoverage() < _targetCoverage) {
      if (_nEvents % 1000000 == 0) {
			std::cout << _nEvents << "\t" << _getCoverage() << std::endl;
			if (crossCheck(false) != 0) {
				crossCheck();
				std::cin.ignore();
			}
      }
		if (_getCoverage() > currTarget) {
			results.addGridState(grid._lattice);
			results.addRow(getResultVector());
			currTarget += _targetCoverage / 10;
		}
      execute();
	}
	crossCheck();
   results.addGridState(grid._lattice);
   results.addRow(getResultVector());

	/*double relaxTime = 50.0;
	double targetTime = evolution._masterTime + relaxTime;

	evolution._rates.ads = 0.0;
	evolution._rates.totalAds = 0.0;

	while (evolution._masterTime < targetTime) {
		if (_nEvents % 1000000 == 0) {
			std::cout << _nEvents << "\t" << _getCoverage() << std::endl;
			if (crossCheck(false) != 0) {
				crossCheck();
				std::cin.ignore();
			}
		}
		execute();
	}
	crossCheck();
	results.addGridState(grid._lattice);
	results.addRow(getResultVector());*/

   return simulationReturn::allright;
}

void Events::execute()
{
	evolve();

newChoose:;

	double chooseEvent = _random() * rates.total;

	if (chooseEvent < rates.totalAds) {
		int index = static_cast<int> (floor(chooseEvent / rates.ads));

		adsorption(index, atomType::metal);
	}
	else if (chooseEvent < rates.totalAds + rates.totalDes) {
		chooseEvent -= rates.totalAds;
		double cumulateRates = 0;
		unsigned ei = 0, ej = 0;
      while (cumulateRates + rates.sumsofDes[ei] < chooseEvent) {
         if (ei >= c::nDesTypes) {
            goto newChoose;
         }
         cumulateRates += rates.sumsofDes[ei++];
      }
      ej = static_cast<int> (floor((chooseEvent - cumulateRates) / rates.des[ei]));
      if (ej >= eventLists._desEvents[ei].size()) {
         goto newChoose;
      }/*

		if (ei % c::nNeighCount == 0)
			_nFreeDes++;

      _nDesorbNeigh[ei % c::nNeighCount]++;*/

		desorption(eventLists._desEvents[ei][ej]);
	}
	else {
		chooseEvent -= rates.totalAds + rates.totalDes;
		double cumulateRates = 0;
		unsigned ei = 0, ej = 0;
      while (cumulateRates + rates.sumsofDiff[ei] < chooseEvent) {
         if (ei >= c::nDiffTypes) {
            goto newChoose;
         }
         cumulateRates += rates.sumsofDiff[ei++];
      }
		ej = static_cast<int> (floor((chooseEvent - cumulateRates) / rates.diff[ei]));
      if (ej >= eventLists._diffEvents[ei].size()) {
         goto newChoose;
      }
		checkDiff(ei, eventLists._diffEvents[ei][ej]);
		diffusion(eventLists._diffEvents[ei][ej]);
   }
   _nEvents++;
}

void Events::executeDes()
{
   evolution._rates.setSums(eventLists._desEvents, eventLists._diffEvents);

	double chooseEvent = _random() * rates.totalDes;
	double cumulateRates = 0;
	int ei = 0, ej = 0;
	while (cumulateRates + rates.sumsofDes[ei] < chooseEvent)
		cumulateRates += rates.sumsofDes[ei++];
	ej = static_cast<int> (floor((chooseEvent - cumulateRates) / rates.des[ei]));

	desorption(eventLists._desEvents[ei][ej]);
}

void Events::evolve()
{
   evolution._rates.setSums(eventLists._desEvents, eventLists._diffEvents);

   double deltaTime = -log(evolution._trandom()) / evolution._rates.total;

   /*while (deltaTime > evolution._timeBetweenHeating) {
      evolution.heating(eventLists._desEvents, eventLists._diffEvents);
      evolution._masterTime += evolution._timeBetweenHeating;
      evolution._timeToHeat = evolution._timeBetweenHeating * (1 + static_cast<int>(evolution._masterTime / evolution._timeBetweenHeating));

      evolution.theta.deltaCovData.emplace_back(evolution._para.T, _getCoverage(), _nDesorbNeigh, getOtherData(), evolution._masterTime);
      deltaTime = -log(evolution._trandom()) / evolution._rates.total;
   }*/
   evolution._masterTime += deltaTime;
   /*if (evolution._masterTime > evolution._timeToHeat && evolution._para.heatingRate > 0.0) {
      evolution.heating(eventLists._desEvents, eventLists._diffEvents);
      evolution._timeToHeat = evolution._timeBetweenHeating * (1 + static_cast<int>(evolution._masterTime / evolution._timeBetweenHeating));

      evolution.theta.deltaCovData.emplace_back(evolution._para.T, _getCoverage(), _nDesorbNeigh, getOtherData(), evolution._masterTime);
   }*/
}

std::vector<double> Events::getResultVector()
{
   return
      std::vector<double> {
      evolution._masterTime,
         static_cast<double>(_getnActions()),
         static_cast<double>(_nMetal),
         static_cast<double>(_nSilicon),
         _getCoverage(),
         static_cast<double>(_nAds),
         static_cast<double>(_nDes),
         0,
         static_cast<double>(_nDiff),
         0,
         0,
         0,//static_cast<double>(grid.cluster.freeAtomCount()),
         0,//static_cast<double>(grid.cluster.clusterCount()),
         evolution._para.T
      };
}

//int Events::getnBorderAtoms()
//{
//   int toReturn = 0;
//   for (int i = 0; i < c::nNeighCount - 1; i++) {
//      toReturn += eventLists._desEvents[i].size() + eventLists._desEvents[i + c::nNeighCount].size();
//   }
//   return toReturn;
//}
//
//int Events::getnFreeAtoms()
//{
//   return eventLists._desEvents[0].size() + eventLists._desEvents[c::nNeighCount].size();
//}
//
//double Events::getEdes()
//{
//   double Edes = 0.0;
//   int count = 0;
//   for (int i = 0; i < c::nDesTypes; i++) {
//      int size = eventLists._desEvents[i].size();
//      count += size;
//      Edes += (((i / c::nNeighCount) ? para.E_Si : para.E_1) * 3 + para.E_1 * (i % c::nNeighCount)) * size;
//   }
//   Edes /= count;
//   return Edes;
//}
//
//std::vector<double> Events::getOtherData()
//{
//   std::vector<double> otherData;
//   otherData.push_back(static_cast<double>(_nDiff));
//   otherData.push_back(getnFreeAtoms());
//   double nBorderAtoms = getnBorderAtoms();
//   otherData.push_back(nBorderAtoms / c::A);
//   otherData.push_back(nBorderAtoms / _nAtom);
//   otherData.push_back(getEdes());
//
//   return otherData;
//}