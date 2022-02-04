// c++ 40 queries in 8.8 sec
// CF  40 queries in 199.786 sec

#include <iostream>
#include <string>
#include "WordService.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main() {
	WordService wordService;
	string sLetterGroup;
	string sWordPattern;

	cout << "wordsearch v0.1" << endl;
	cout << "" << endl;
	cout << "First, type every letter in your hand (e.g., if you have a letter twice, type it twice). Skip wild letters in this step. " << endl;
	cout << "Letter Group #1: ";
	cin >> sLetterGroup;
	cout << endl << "0 = wild letter" << endl;
	cout << "1 = instance of letter from group" << endl;
	cout << "[a-z] = any static letter" << endl;

	while( true ) {
		cout << "Word pattern: ";
		cin >> sWordPattern;

		// const clock_t cStart = clock();
	
		vector<string> vFinalWordMatches = wordService.findWordMatches( sWordPattern, sLetterGroup );

		if ( vFinalWordMatches.size() > 0 ){
			for( vector<string>::iterator itWord = vFinalWordMatches.begin(); itWord != vFinalWordMatches.end(); ++itWord) {
				cout << endl << *itWord << endl;
			}
		} else {
				cout << "No matches found." << endl << endl;
		}

		// const clock_t cEnd = clock();
		// cout << ( static_cast<double>(cEnd - cStart) / CLOCKS_PER_SEC )<< endl;
	}

	return 0;
}
