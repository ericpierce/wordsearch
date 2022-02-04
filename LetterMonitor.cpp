#include <iostream>
#include "LetterMonitor.h"

using namespace std;

LetterMonitor::LetterMonitor() 
{
	init();
}

// Default destructor
LetterMonitor::~LetterMonitor() {}

void LetterMonitor::init()
{
	available = 0;
	found = 0;
}

void LetterMonitor::setAvailable( int iAvailable ){
	available = iAvailable;
}

void LetterMonitor::setFound( int iFound){
	found = iFound;
}

int LetterMonitor::getAvailable(){
	return available;
}

int LetterMonitor::getFound(){
	return found;
}
