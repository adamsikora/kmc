#include "Eventlist.h"

void EventLists::properDesEventReplace(PointerToEvent ptrToEvent)      // replaces outdated desorption event
{
	int rate = ptrToEvent.getRate(), position = ptrToEvent.getPosinVector();
	desorbPointers[_desEvents[rate][position].src].unset();
	if (static_cast<unsigned> (position) < _desEvents[rate].size() - 1) {
		_desEvents[rate][position] = _desEvents[rate].back();
		desorbPointers[_desEvents[rate][position].src].set(ptrToEvent);
	}
	_desEvents[rate].pop_back();
}

void EventLists::properDiffEventReplace(PointerToEvent ptrToEvent)     // replaces outdated diffusion event
{
	const int rate = ptrToEvent.getRate(), position = ptrToEvent.getPosinVector();
	SD* const pomsd = &_diffEvents[rate][position];
	srcPointers[pomsd->srcnDir].unset();
	destPointers[pomsd->destnDir].unset();
	if (static_cast<unsigned> (position) < _diffEvents[rate].size() - 1) {
		_diffEvents[rate][position] = _diffEvents[rate].back();
		srcPointers[pomsd->srcnDir].set(ptrToEvent);
		destPointers[pomsd->destnDir].set(ptrToEvent);
	}
	_diffEvents[rate].pop_back();
}

void EventLists::properDesEventAddition(int rate, int source)     // adds new desorption event
{
	_desEvents[rate].emplace_back(source);
	desorbPointers[source].set(rate, (_desEvents[rate].size() - 1));
}

void EventLists::properDiffEventAddition(int rate, SD sourcenDest)// adds new diffusion event
{
	_diffEvents[rate].push_back(sourcenDest);
   srcPointers[sourcenDest.srcnDir].set(rate, (_diffEvents[rate].size() - 1));
	destPointers[sourcenDest.destnDir].set(rate, (_diffEvents[rate].size() - 1));
}