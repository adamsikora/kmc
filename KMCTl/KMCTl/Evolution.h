/*
author: Adam Sikora

defines class for managing time and temperature evolution of the system

*/

#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "Includes.h"

#include "mtrand.h"

//#include "Theta.h"
#include "Results.h"
#include "Rates.h"

class Events;

class Evolution
{
public:
   Evolution(const Parameters& para);

   /*void pushThetaDCD(double currCoverage, const std::vector<int>&nDesNeigh, std::vector<double> otherData);

   inline void Evolution::heating(std::vector<S>* desEv, std::vector<SD>* diffEv)
   {
      _para.T += _temperatureStep;
      _rates.setRates(_para);
      _rates.setSums(desEv, diffEv);
   }*/

   friend class Events;

private:
   Rates _rates;

   double _masterTime;

   MTRand_open _trandom;

   Parameters _para;

   /*double _initialTemperature;
   double _temperatureStep;
   double _timeBetweenHeating;
   double _timeToHeat;

   Theta theta;*/


};

#endif // EVOLUTION_H