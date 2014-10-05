#include "Evolution.h"


Evolution::Evolution(const Parameters& para) : _para(para), _rates()//, theta(para)
{

   /*_initialTemperature = para.T;
   _temperatureStep = c::temperatureStep;
   _timeBetweenHeating = _temperatureStep / para.heatingRate;
   _timeToHeat = 0.0;*/

   _rates.setRates(_para);
}

//void Evolution::pushThetaDCD(double currCoverage, const std::vector<int>&nDesNeigh, std::vector<double> otherData)
//{
//   theta.deltaCovData.emplace_back(_para.T, currCoverage, nDesNeigh, otherData, _masterTime);
//}