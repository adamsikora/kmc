#include "State.h"

State::State(Parameters para, Results result, Grid grid) : parameters(para), results(result), width(c::w), height(c::h)
{
	lattice.reserve(c::A);
	for (int i = 0; i < c::A; ++i) {
		lattice.push_back(grid.present(i));
	}
}

void State::alterState(Parameters para, Results result, Grid grid)
{
	parameters = para;
	//results = result;
	for (int i = 0; i < c::A; ++i) {
		lattice[i] = grid.present(i);
	}
}


void State::loadState(std::string file)
{

}

void State::saveState(std::string file)
{

}