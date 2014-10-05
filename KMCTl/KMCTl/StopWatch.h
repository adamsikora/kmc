/*
author: Adam Sikora

defines class for simple measuring of computation time

header only

*/

#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "Includes.h"

class StopWatch
{
public:
	StopWatch() : startClock(0), stopClock(0), lastElapsed(0), totalElapsed(0) {}
	void start() { startClock = clock(); }
	void stop()  { stopClock = clock(); totalElapsed += lastElapsed = (stopClock - startClock) / static_cast<double> (CLOCKS_PER_SEC); }

	std::string getLastElapsed()  { return convertToTime(lastElapsed);  }   // outputs time of last measured sequence ( hh:mm:ss.ss )
	std::string getTotalElapsed() { return convertToTime(totalElapsed); }   // outputs total accumulated time of given stop watch

private:
	clock_t startClock, stopClock;
	double lastElapsed, totalElapsed;

	std::string convertToTime(double t)    // returns formatted time
	{
		double seconds = t;

		int hours = static_cast<int> (seconds / 3600);
		seconds -= hours * 3600;
		int minutes = static_cast<int> (seconds / 60);
		seconds -= minutes * 60;

		std::stringstream ss;
		ss << std::fixed << std::setfill('0')
			<< std::setw(2) << hours << ":"
			<< std::setw(2) << minutes << ":"
			<< std::setw(5) << std::setprecision(2) << seconds;

		return ss.str();
	}

};

#endif //STOPWATCH_H