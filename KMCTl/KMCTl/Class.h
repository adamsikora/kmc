/*
author: Adam Sikora

defines basic classes

just header

*/

#ifndef CLASS_H
#define CLASS_H



class Change
{
public:
   Change(int old, int actual, int position) :
      old(old), actual(actual), position(position) {}
   int old, actual, position;

private:

};

class PointerToEvent
{
public:
	PointerToEvent(int point = -1) :  posinVector(point), rate(point) {}

   inline int getRate() const { return rate; }
   inline int getPosinVector() const { return posinVector; }
   inline void set(int rate, int posinVector) { this->rate = rate; this->posinVector = posinVector; }
   inline void set(const PointerToEvent& ptrToEvent) { this->rate = ptrToEvent.rate; this->posinVector = ptrToEvent.posinVector; }
	inline void unset() {posinVector = -1; rate = -1;}
   inline bool isValid() const { return (posinVector != c::invalidPointer); }

private:
   int posinVector, rate;

};


#endif // CLASS_H