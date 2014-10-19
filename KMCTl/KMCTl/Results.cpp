#include "Results.h"

Results::Results(double deltaCov) : deltaCoverage(deltaCov), currDepoTargetCoverage(0.0), currDesTargetCoverage(1.0)
{
   dataColumns.emplace_back("time    ", 2, std::ios_base::fmtflags(std::ios::scientific));
   dataColumns.emplace_back("actions", 0, std::ios_base::fmtflags(std::ios::fixed));
   dataColumns.emplace_back("Matoms", 0, std::ios_base::fmtflags(std::ios::fixed));
   dataColumns.emplace_back("Siatoms", 0, std::ios_base::fmtflags(std::ios::fixed));
   dataColumns.emplace_back("cover", 3, std::ios_base::fmtflags(std::ios::fixed));
   dataColumns.emplace_back("nAds", 0, std::ios_base::fmtflags(std::ios::fixed));
   dataColumns.emplace_back("nDes", 0, std::ios_base::fmtflags(std::ios::fixed));
   dataColumns.emplace_back("nFreDes", 0, std::ios_base::fmtflags(std::ios::fixed), false);
   dataColumns.emplace_back("nDiff", 0, std::ios_base::fmtflags(std::ios::fixed));
   dataColumns.emplace_back("nFreDif", 0, std::ios_base::fmtflags(std::ios::fixed), false);
   dataColumns.emplace_back("stepDif", 0, std::ios_base::fmtflags(std::ios::fixed), false);
   dataColumns.emplace_back("freAtom", 0, std::ios_base::fmtflags(std::ios::fixed), false);
   dataColumns.emplace_back("cluster", 0, std::ios_base::fmtflags(std::ios::fixed), false);
   dataColumns.emplace_back("temper", 0, std::ios_base::fmtflags(std::ios::fixed), false);
}

bool Results::timeToAdd(double currCoverage)
{
   if (deltaCoverage == noDeltaCovTracking)
      return false;
   if (currCoverage < currDesTargetCoverage) {
      currDesTargetCoverage -= deltaCoverage;
      return true;
   }
   else
      return false;
}

void Results::addRow(std::vector<double> dataRow)
{
   if (dataRow.size() != dataColumns.size()) {
      std::cout << "wrong data to results";
      std::cin.ignore();
   }
   for (unsigned i = 0; i < dataRow.size(); i++)
      dataColumns[i].values.push_back(dataRow[i]);
}

std::string Results::getHeadRow()
{
   std::stringstream ss;

   for (std::vector<DataColumn>::iterator it = dataColumns.begin(); it != dataColumns.end(); it++) {
      if (it->inUse) {
         ss << it->name << "\t";
      }
   }
   return ss.str();
}

std::string Results::getAllRows()
{
   std::stringstream ss;

   for (unsigned i = 0; i < dataColumns[0].values.size(); i++) {
      for (std::vector<DataColumn>::iterator it = dataColumns.begin(); it != dataColumns.end(); it++) {
         if (it->inUse) {
            ss.setf(it->flags);
            ss << std::setprecision(it->precision);
            ss << it->values[i] << "\t";
            ss.unsetf(std::ios_base::floatfield);
         }
      }
      ss << std::endl;
   }

   return ss.str();
}

std::string Results::getLastRow()
{
   std::stringstream ss;

   for (std::vector<DataColumn>::iterator it = dataColumns.begin(); it != dataColumns.end(); it++) {
      if (it->inUse) {
         ss.setf(it->flags);
         ss << std::setprecision(it->precision);
         ss << it->values.back() << "\t";
         ss.unsetf(std::ios_base::floatfield);
      }
   }
   return ss.str();
}
//
//std::vector<std::vector<bool>> Results::getBMPs(bool all)
//{
//   std::vector<std::vector<bool>> toReturn;
//   for (auto state = all ? gridStates.begin() : gridStates.end() - 1; state != gridStates.end(); state++) {
//      std::vector<bool> BMP;
//      unsigned gridIndex = 0;
//      for (int i = 0; i < c::A; i++) {
//         if ((i / c::w) % 2 == 0) {
//            BMP.push_back((*state)[gridIndex].height > 0);
//         }
//         else {
//            BMP.push_back((*state)[gridIndex].height + (*state)[gridIndex - 1].height > 0);
//         }
//         if (i % (c::w * 2) != c::w * 2 - 1) {
//            gridIndex++;
//         }
//      }
//      toReturn.push_back(BMP);
//   }
//
//   return toReturn;
//}

std::vector<std::string> Results::getPDBs(bool all, bool zkrychlit)
{
   std::vector<std::string> toReturn;

   for (auto state = (all || gridStates.empty()) ? gridStates.begin() : gridStates.end() - 1; state != gridStates.end(); state++) {
      std::stringstream PDB;
      PDB.setf(std::ios_base::right | std::ios_base::fixed);

      double dist = 3.450;
      int line = 1;
      double x, y, z, t;
		for (int pi = 0; pi < c::h; pi++) {
			for (int pj = 0; pj < c::w; pj++) {
				x = dist*(pj + 0.5*(pi - 1));
				y = -sqrt(3.0) / 2.0*dist*(pi + 1.0 / 3.0*(-1));
				z = sqrt(2.0 / 3.0)*dist*(-1);
				t = 0.1 * 0;

				if (zkrychlit) {
					if ((pi + 1) / 2 + pj > c::w - 1) {
						x -= c::w * dist - 0.5 * dist;
						y -= sqrt(3.0) / 2.0 * dist;
					}
				}
				PDB << "ATOM  "
					<< std::setw(5) << line++ << " Tl   UNK     0    "
					<< std::setw(8) << std::setprecision(3) << x
					<< std::setw(8) << std::setprecision(3) << y
					<< std::setw(8) << std::setprecision(3) << z
					<< std::setw(6) << std::setprecision(2) << 0.0
					<< std::setw(6) << std::setprecision(2) << t
					<< "  0" << "\n";
			}
		}

		for (int pi = 0; pi < c::h; pi++) {
			for (int pj = 0; pj < c::w; pj++) {
				if ((*state)[pi * c::w + pj].getAtom() != atomType::nothing) {
					x = dist*(pj + 0.5*pi);
					y = -sqrt(3.0) / 2.0*dist*pi;
					z = 0.0;
					t = (*state)[pi * c::w + pj].getAtom();

					if (zkrychlit) {
						if ((pi + 1) / 2 + pj > c::w - 1) {
							x -= c::w * dist - 0.5 * dist;
							y -= sqrt(3.0) / 2.0 * dist;
						}
					}
					PDB << "ATOM  "
						<< std::setw(5) << line++ << ((*state)[pi * c::w + pj].getAtom() == atomType::metal ? " Mn" : " Si") << "   UNK     0    "
						<< std::setw(8) << std::setprecision(3) << x
						<< std::setw(8) << std::setprecision(3) << y
						<< std::setw(8) << std::setprecision(3) << z
						<< std::setw(6) << std::setprecision(2) << 0.0
						<< std::setw(6) << std::setprecision(2) << t
						<< "  0" << "\n";
				}
			}
		}

      toReturn.push_back(PDB.str());
   }

   return toReturn;
}


const std::string pdbExtension = ".txt.pdb";
const std::string bmpExtension = ".bmp";
const std::string txtExtension = ".txt";

const std::string pdbName = "pdbMorfology";
const std::string bmpName = "morfology";
const std::string dataName = "dataOutput";
const std::string thetaName = "theta";
const std::string paraName = "parameters";
const std::string resultName = "results";
const std::string reportName = "report";

const std::string mainFolder = ".\\Simulations\\";

Report::Report(Parameters para, double deltaCov) : parameters(para), results(deltaCov)
{
   
}

std::string Report::getCharacteristic()
{
   std::stringstream ss;
   ss.setf(std::ios::fixed);
   ss << "(" << c::w << ","
      << std::setprecision(2) << parameters.E_M_M << ","
		<< std::setprecision(2) << parameters.E_bM << ","
		<< std::setprecision(2) << parameters.E_b100 << ","
		<< std::setprecision(2) << parameters.E_b111 << ","
      << parameters.seed << ")";
   return ss.str();
}

void Report::putFile(const std::string& name, const std::string& content, std::ios_base::openmode mode)
{
   std::ofstream fout;

   fout.open(name, std::ios_base::out | mode);

   if (!fout.is_open()) {
      std::cout << "malo pameti";
   }
   fout << content;

   fout.close();
}

bool Report::fileExists(std::string name)
{
   std::string control = name;
   std::ifstream f(control.c_str());
   if (f.good()) {
      f.close();
      return true;
   }
   else {
      f.close();
      return false;
   }
}

void Report::pdbput(std::string charakteristic, bool all, bool zkrychlit)
{
   std::vector<std::string> PDBs = results.getPDBs(all, zkrychlit);
   int i = 1;

   for (auto it : PDBs) {
      putFile(path + pdbName + charakteristic + /*std::to_string(i++) +*/ pdbExtension, it);
   }
}

//void Report::bmpput(std::string charakteristic, bool all)
//{
//   std::vector<std::vector<bool>> Grids = results.getBMPs(all);
//   int next = 1;
//
//   BMP bmp;
//   bmp.SetSize(c::w, c::h);
//   bmp.SetBitDepth(1);
//
//   for (auto it : Grids) {
//      for (int i = 0; i < c::w; i++) {
//         for (int j = 0; j < c::h; j++) {
//            bmp.SetPixel(i, j, bmp.GetColor((int)it[j*c::w + i]));
//         }
//      }
//      std::string name = path + bmpName + charakteristic + std::to_string(next++) + bmpExtension;
//      bmp.WriteToFile(name.c_str());
//   }
//}

void Report::reportput(std::string characteristic)
{
   std::string name = path + reportName + characteristic + txtExtension;
   if (!fileExists(name)) {
      putFile(name, getAllHead() + getFinal());
   }
   else {
      putFile(name, getFinal(), std::ios_base::app);
   }
   name = mainFolder + reportName + characteristic + txtExtension;
   if (!fileExists(name)) {
      putFile(name, getAllHead() + getFinal());
   }
   else {
      putFile(name, getFinal(), std::ios_base::app);
   }
}

void Report::saveIt(std::string characteristic, std::string fileName)
{
   //if (fileName == "") {
   //   path = mainFolder + getCharacteristic() + "\\";
   //}
   //else {
   //   path = mainFolder + fileName + "\\";
   //}
	//
   //boost::filesystem::create_directories(path);
	//
   //pdbput(characteristic, true);
   ////bmpput(characteristic);
	//
   //reportput(std::to_string(c::w));
}