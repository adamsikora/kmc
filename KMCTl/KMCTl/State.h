#include "Includes.h"
#include "Input.h"
#include "Results.h"
#include "Grid.h"

class State {
public:
	State(Parameters para, Results result, Grid grid);
	void alterState(Parameters para, Results result, Grid grid);
	void loadState(std::string file);
	void saveState(std::string file);

private:
	Parameters parameters;
	Results results;
	int64_t width, height;
	std::vector<int64_t> lattice;

};