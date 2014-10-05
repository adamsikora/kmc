/*
author: Adam Sikora

defines class for managing simulation output

*/

#ifndef RESULTS_H
#define RESULTS_H

#include <direct.h>
#include "Includes.h"
#include "Grid.h"

//#include "boost\filesystem.hpp"

//#include "EasyBMP\EasyBMP.h"


class Parameters     // class for storing simulation parameters
{
public:
   Parameters() :
		nu(0), T(0), E_M_M(0.0), E_M_Si(0.0), E_M_Tl(0.0), E_bM(0.0), E_b100(0.0), E_b111(0.0),
      layersPerSec(0), targetCoverage(0.0), seed(0) {}
   Parameters
      (
		double nu, double T, double E_M_M, double E_M_Si, double E_M_Tl, double E_bM, double E_b100, double E_b111,
      double layersPerSec, double targetCoverage, int seed
      ) :
		nu(nu), T(T), E_M_M(E_M_M), E_M_Si(E_M_Si), E_M_Tl(E_M_Tl), E_bM(E_bM), E_b100(E_b100), E_b111(E_b111),
      initialHeight(initialHeight), layersPerSec(layersPerSec), targetCoverage(targetCoverage), heatingRate(heatingRate), seed(seed) {}

   std::string getHeadRow() { return "nu        \tT\tE_M_M\tE_M_Si\tE_M_Th\tE_Si_Si\tE_Si_Th\tE_bSi\tE_bM\tlps\ttargCov\tseed\t"; }
   std::string getRow()
   {
      std::stringstream ss;

      ss << std::scientific << std::setprecision(1) << nu << "\t"
         << std::defaultfloat << std::fixed << std::setprecision(0) << T << "\t"
         << std::setprecision(3) << E_M_M   << "\t"
         << std::setprecision(3) << E_M_Si  << "\t"
         << std::setprecision(3) << E_M_Tl  << "\t"
			<< std::setprecision(3) << E_bM    << "\t"
			<< std::setprecision(3) << E_b100  << "\t"
			<< std::setprecision(3) << E_b111  << "\t"
         << layersPerSec << "\t"
         << std::setprecision(3) << targetCoverage << "\t" << seed << "\t";

      return ss.str();
   }
   std::string getAll() { return getHeadRow() + "\n" + getRow(); }

   double nu, T;

	double E_M_M, E_M_Si, E_M_Tl, E_bM, E_b100, E_b111;

   int initialHeight;
   double layersPerSec, targetCoverage, heatingRate;

   int seed;

};

class DataColumn     // class for storing graph outputs
{
public:
   DataColumn(std::string name, int precision, std::ios_base::fmtflags flags, bool inUse = true) :
      name(name), precision(precision), flags(flags), inUse(inUse) {};

   std::string name;
   std::vector<double> values;
   int precision;
   std::ios_base::fmtflags flags;
   bool inUse;

private:

};

const double noDeltaCovTracking = 1111.0;

class Results     // class for storing all results
{
public:
   Results(double deltaCov = noDeltaCovTracking);

   bool timeToAdd(double currCoverage);
   void addRow(std::vector<double> dataRow);
   std::string getHeadRow();
   std::string getLastRow();
   std::string getAllRows();

   inline void addGridState(const std::vector<GridCell>& lattice)     // adds current grid state for graphical output
   {
      gridStates.push_back(lattice);
   }
   std::vector<std::string> getPDBs(bool all = true, bool zkrychlit = true);  // gets data for output for jMol
   std::vector<std::vector<bool>> getBMPs(bool all = true);                   // gets data for bmp output

private:
   std::vector<DataColumn> dataColumns;

   const double deltaCoverage;
   double currDesTargetCoverage, currDepoTargetCoverage;

   std::vector<std::vector<GridCell>> gridStates;

};

class Report      // unifies output data
{
public:
   Report(Parameters para, double deltaCov = noDeltaCovTracking);
   std::string getAllHead() { return results.getHeadRow() + parameters.getHeadRow() + "\n"; }
   std::string getFinal() { return results.getLastRow() + parameters.getRow() + "\n"; }
   std::string getCharacteristic();

   Parameters parameters;
   Results results;

   std::string path;

   void saveIt(std::string characteristic, std::string fileName = "");  // saves the data

private:
   void putFile(const std::string& name, const std::string& content, std::ios_base::openmode mode = std::ios_base::out);   // saves content into file
   bool fileExists(std::string name);     // checks whether given file already exists

   void pdbput(std::string charakteristic, bool all = true, bool zkrychlit = true);    // outputs data for jMol
   //void bmpput(std::string charakteristic, bool all = true);                           // outputs morfology in bmp
   void reportput(std::string characteristic);                                         // outputs txt file with results

};

#endif // RESULTS_H