/*
author: Adam Sikora

defines class for storing event rates

header only

*/

#ifndef RATES_H
#define RATES_H

#include "Includes.h"
#include "Results.h"

class Rates
{
public:
	Rates()
   {
		for (int ci = 0; ci < c::nDesTypes; ci++)
			sumsofDes[ci] = 0;
	
		for (int ci = 0; ci < c::nDiffTypes; ci++)		
				sumsofDiff[ci] = 0;
		totalAds = totalDes = totalDiff = total = 0;
	}

	std::string const getRate(int srcVicinity, int destVicinity, int type)
	{
		std::stringstream ss;
		ss << "src: " << srcVicinity;
		ss << " dest: " << destVicinity;
		ss << " type: " << type;
		ss << " index: " << c::getDiffType(srcVicinity, destVicinity, type);
		ss << " rate: " << diff[c::getDiffType(srcVicinity, destVicinity, type)] << std::endl;
		return ss.str();
	}


	void setRates(const Parameters& para)
	{
		ads = para.layersPerSec;
      for (int si = 0; si < c::nNeighCount; si++) {
         for (int mi = 0; mi < c::nNeighCount; mi++) {
            double E_pom = 3 * para.E_M_Tl + si * para.E_M_Si + mi * para.E_M_M;
            des[si*c::nNeighCount + mi] = para.nu * exp(-E_pom / (c::boltzmann*para.T));

            /*E_pom = 3 * para.E_Si_Th + si * para.E_Si_Si + mi * para.E_M_Si;         //for desorption of Si atoms
            des[c::nVicinityTypes + si*c::nNeighCount + mi] = para.nu * exp(-E_pom / (c::boltzmann*para.T));*/
         }
      }
	
      for (int si = -3; si < 4; si++) {   //si = Sidest - Sisrc
         for (int mi = -3; mi < 4; mi++) {   //mi = Mdest - Msrc
				int posInVec = (si + 3) * c::nDiffChanges + (mi + 3);
				double delta_E = si * para.E_M_Si + mi * para.E_M_M;
				//double deltas = -delta_E / 2 + delta_E*delta_E / (16 * para.E_bM);
				double barrier = para.E_bM;
				double E_pom = barrier - delta_E / 2 + delta_E*delta_E / (16 * barrier);	// no edge neighbors
            diff[posInVec]  // (Sidest-Sisrc+3)*nDiffChanges + Mdest-Msrc+3
               = para.nu*exp(-E_pom / (c::boltzmann*para.T));

				barrier = para.E_bM + para.E_b100;		// 100 edge neighbor
				E_pom = barrier - delta_E / 2 + delta_E*delta_E / (16 * barrier);
				diff[c::nDiffChanges * c::nDiffChanges + posInVec]  // (Sidest-Sisrc+3)*nDiffChanges + Mdest-Msrc+3
					= para.nu*exp(-E_pom / (c::boltzmann*para.T));

				barrier = para.E_bM + para.E_b111;		// 111 edge neighbor
				E_pom = barrier - delta_E / 2 + delta_E*delta_E / (16 * barrier);
				diff[2 * c::nDiffChanges * c::nDiffChanges + posInVec]  // (Sidest-Sisrc+3)*nDiffChanges + Mdest-Msrc+3
					= para.nu*exp(-E_pom / (c::boltzmann*para.T));

				barrier = para.E_bM + para.E_b100 + para.E_b111;		// both edge neighbors
				E_pom = barrier - delta_E / 2 + delta_E*delta_E / (16 * barrier);
				diff[3 * c::nDiffChanges * c::nDiffChanges + posInVec]  // (Sidest-Sisrc+3)*nDiffChanges + Mdest-Msrc+3
					= para.nu*exp(-E_pom / (c::boltzmann*para.T));

            /*double delta_E = 3 * para.E_Si_Th + si * para.E_Si_Si + mi * para.E_M_Si;      // for diffusion of Si atoms
            double E_pom = para.E_bSi - delta_E / 2 + delta_E*delta_E / (16 * para.E_bSi);
            diff[c::nDiffChanges*c::nDiffChanges + (si + 3) * c::nDiffChanges + (mi + 3)] = para.nu*exp(-E_pom / (c::boltzmann*para.T));*/
         }
      }
	}

	void setSums(std::vector<S>* desEv, std::vector<SD>* diffEv)
	{
		totalDes = totalDiff = total = 0;
		totalAds = ads*c::w*c::h;

		for (int ci = 0; ci < c::nDesTypes; ci++)
			if (!desEv[ci].empty())
				totalDes += sumsofDes[ci] = des[ci] * desEv[ci].size();
			else
				sumsofDes[ci] = 0;

		for (int ci = 0; ci < c::nDiffTypes; ci++)
			if (!diffEv[ci].empty())
				totalDiff += sumsofDiff[ci] = diff[ci] * diffEv[ci].size();
			else
				sumsofDiff[ci] = 0;
	
		total = totalAds + totalDes + totalDiff;
   }

   double ads;
   double des[c::nDesTypes];
   double diff[c::nDiffTypes];

   double sumsofDes[c::nDesTypes], sumsofDiff[c::nDiffTypes];
   double totalAds, totalDes, totalDiff;
   double total;
};

#endif //RATES_H