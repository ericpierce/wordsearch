#include <map>
#include <string>
#include <vector>
#include "LetterMonitor.h"

#ifndef WORDSERVICE_H_
#define WORDSERVICE_H_

using std::map;
using std::string;
using std::vector;

class WordService {
	public:
		vector<string> findWordMatches( string, string );
	private:
		vector<string> findWordRegexMatches( string );
		vector<string> filterOutFalseWordMatches( string, map<int, string>, vector<string>  );
		string generateWordPatternRegex( string, map<int, string> );
		string removeLetterGroupDupes( string );
		map<char, LetterMonitor> getLetterOccurance( string );
		bool validateLetterTracker( map< int, map< char, LetterMonitor > > );
};

#endif /*WORDSERVICE_H_*/
