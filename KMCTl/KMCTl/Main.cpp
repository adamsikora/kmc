#include "Input.h"
#include "Events.h"
#include "StopWatch.h"

int main()
{
	StopWatch sw;
	sw.start();

   Parameters pomPara(inputPara);

   std::cout << pomPara.getAll();

   Report report(pomPara);

   Events events(report);
   events.growthSimulation();

   report.saveIt("");
   
   std::cout << report.results.getHeadRow() << std::endl;
   std::cout << report.results.getAllRows() << std::endl << std::endl;
   std::cout << "\n\n\n";

	sw.stop();
	std::cout << "Simulation completed, it took: " << sw.getTotalElapsed() << "\n";

	std::cin.ignore();

	return EXIT_SUCCESS;
}