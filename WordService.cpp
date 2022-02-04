#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <boost/regex.hpp> // Boost.Regex lib
#include <boost/config.hpp>
#include "LetterMonitor.h"
#include "WordService.h"
#include "WordUtil.h"

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

/* public */
vector<string> WordService::findWordMatches( string sWordPattern, string sLetterGroup ){
	map<int, string> maLetterGroup;
	maLetterGroup[0] = "abcdefghijklmnopqrstuvwxyz";	// Here's what we use for the blank tile.
	maLetterGroup[1] = sLetterGroup;					// Add on letter group passed in.

	// Main steps to find word matches
	string sRegex = generateWordPatternRegex( sWordPattern, maLetterGroup );
	vector<string> vRegExWordMatches = findWordRegexMatches( sRegex );
	vector<string> vFinalWordMatches = filterOutFalseWordMatches( sWordPattern, maLetterGroup, vRegExWordMatches );

	return vFinalWordMatches;
}

/* private */
string WordService::generateWordPatternRegex( string sWordPattern, map<int, string> maLetterGroup ){
	unsigned short nIndex;
	string sLetterGroup;
	string sWordPatternRegex = "^";	// start regex w/start of word

	// iterate over each character of word pattern
	for ( nIndex = 0; nIndex < sWordPattern.length(); nIndex++ ) {
		 
		if ( isdigit( sWordPattern[ nIndex ] ) ) {	// Check for a numeral which indicates a letter group index.
			// char cWordChar = sWordPattern[ nIndex ];
			// TODO: don't hit removeLetterGroupDupes every iteration.  Do it once for each letter group outside of this for loop.
			// Get the letter group associated w/this letter group index
			int iWordChar = sWordPattern[ nIndex ] - '0';	// convert char to integer.  This is the letter group index.

			sLetterGroup = removeLetterGroupDupes( maLetterGroup[ iWordChar ] );	// Pass in letter group associated w/this numeric key.

			// Add in regexified letter group.
			sWordPatternRegex.append( "[" );
			sWordPatternRegex.append( sLetterGroup );
			sWordPatternRegex.append( "]{1}");
		} else if ( isalpha( sWordPattern[ nIndex ] ) ) {
			// If simple letter, add the letter to the regex as is.
			sWordPatternRegex.append( 1, sWordPattern[ nIndex ] );

		} else {
			// TODO: Should not come here. Probably should validate word pattern before iterating over it.
			// throw();
		}
	}

	sWordPatternRegex.append("$");	// end regex w/end of word

	// cout << "sWordPatternRegex: " << sWordPatternRegex << endl;

	return sWordPatternRegex;
}

vector<string> WordService::filterOutFalseWordMatches( const string sWordPattern, map<int, string> maLetterGroup, vector<string> veWordMatches ){
	vector<string> veValidatedWordMatches;
	unsigned short nIndex;
	map<int, string>::iterator itLetterGroup;
	string sLetterPattern;

	// Loop through words
	for( vector<string>::iterator itCurrentWord = veWordMatches.begin(); itCurrentWord != veWordMatches.end(); ++itCurrentWord ) {
		// cout << " -> checking word: " << *itCurrentWord << endl;

		map< int, map< char, LetterMonitor > > maLetterGroupTracker;
		/*
		* Example of what this map looks like:
		*	maLetterGroupTracker[1]['e'].getAvailable() = 2
		*	
		*	The index matches the maLetterGroup index.  E.g., 1 = 'elaekdp' (a group of letters)
		*	And the letter ('e') is a letter from the letter group to hold how many of that letter are available ( via getAvailable() )
		*   and a counter to keep track of how many found ( via getFound() ) for the word being checked.
		*/

		// Iterate through letter groups; create a new map of letter groups that contains the available and found (a holder for now) letters for each letter group.
		for ( itLetterGroup = maLetterGroup.begin(); itLetterGroup != maLetterGroup.end(); ++itLetterGroup ) {
			maLetterGroupTracker[ itLetterGroup->first ] = getLetterOccurance( itLetterGroup->second );
		}

		// Iterate over letters of current word
		for ( nIndex = 0; nIndex < (*itCurrentWord).length(); nIndex++ ) {
			if ( isdigit( sWordPattern[ nIndex ] ) ) { 	// Check for a numeral which indicates a letter group index.
				int iWordChar = sWordPattern[ nIndex ] - '0';	// convert char to integer.  This is the letter group index.

				// track the current letter in tracker map by incrementing its found count.
				maLetterGroupTracker[ iWordChar ][ (*itCurrentWord)[nIndex] ].setFound( maLetterGroupTracker[ iWordChar ][ (*itCurrentWord)[nIndex] ].getFound() + 1 );
			} else {
				// Skip individual letter check since the regex should have gotten it right (go ahead and check if you want!)
			}
		}

		if ( validateLetterTracker( maLetterGroupTracker ) ) {
			// tracker validates; add word to validated matches.
			veValidatedWordMatches.push_back( *itCurrentWord );
		}
	}

	return veValidatedWordMatches;
}

bool WordService::validateLetterTracker( map< int, map< char, LetterMonitor > > maLetterGroupTracker ) {
	bool bTrackerValid = true;
	map< int, map< char, LetterMonitor > >::iterator itLetterGroupTracker;

	// Iterate through letter groups
	for ( itLetterGroupTracker = maLetterGroupTracker.begin(); itLetterGroupTracker != maLetterGroupTracker.end(); ++itLetterGroupTracker ) {
		map< char, LetterMonitor >::iterator itLetterGroup;

		// Iterate through one letter group's letters
		for ( itLetterGroup = itLetterGroupTracker->second.begin(); itLetterGroup != itLetterGroupTracker->second.end(); ++itLetterGroup ) {

			// check if current letter has more found than available; if so invalidate tracker.
			if ( itLetterGroup->second.getFound() > itLetterGroup->second.getAvailable() ) {
				bTrackerValid = false;
				break;
			}
		}

		if( ! bTrackerValid ) {
			// No need to continue validating any more letter groups once a group has a letter showing up too many times.
			break;
		}
	}

	return bTrackerValid;
}

map<char, LetterMonitor> WordService::getLetterOccurance( string sLetterGroup ) {
	map<char, LetterMonitor> maLetterMonitor;
	unsigned short nIndex;

	// Iterate through the letters in the letter group
	for ( nIndex = 0; nIndex < sLetterGroup.length(); nIndex++ ) {
		if( maLetterMonitor[ sLetterGroup[ nIndex ] ].getAvailable() > 0 ){
			// Letter found again; add one more to the available count for this letter.
			maLetterMonitor[ sLetterGroup[ nIndex ] ].setAvailable( 1 + maLetterMonitor[ sLetterGroup[ nIndex ] ].getAvailable() );
		} else {
			// Init the struct for the letter.
			LetterMonitor letterMonitor;
			maLetterMonitor[ sLetterGroup[ nIndex ] ] = letterMonitor;
			maLetterMonitor[ sLetterGroup[ nIndex ] ].setAvailable( 1 );
		}
	}

	return maLetterMonitor;
}

string WordService::removeLetterGroupDupes( string sLetterGroup ){
	string sLetterGroupFiltered = "";
	map<char, bool> maFoundLetters;
	unsigned short nIndex;

	// iterate over each letter
	for ( nIndex = 0; nIndex < sLetterGroup.length(); nIndex++ ) {

		// check if letter has not been found yet
		if( ! maFoundLetters[ sLetterGroup[ nIndex ] ] ){
			maFoundLetters[ sLetterGroup[ nIndex ] ] = true;	// symbolically setting to true; not used though.

			// Add letter to response string
			sLetterGroupFiltered.append( 1, sLetterGroup[ nIndex ] );
		}
	}

	return sLetterGroupFiltered;
}

vector<string> WordService::findWordRegexMatches( string sRegex ) {
	WordUtil wordUtil;
	boost::regex oRegex;
	vector<string> veWordMatches;
	int nLoop = 0;

	try {
		// oRegex.assign( sRegex, boost::regex_constants::icase );	// make case insensitive
		oRegex.assign( sRegex );

		// Iterate over entire word list
		while( wordUtil.words[ nLoop ] != 0 ){

			// Check if regex matches current word
			if ( boost::regex_match(  wordUtil.words[ nLoop ], oRegex ) ) {
				veWordMatches.push_back( wordUtil.words[ nLoop ] );
			}
			nLoop++;
		}
	} catch (boost::regex_error& e) {
		cout << sRegex << " is not a valid regular expression: \"" << e.what() << "\"" << endl;
	}

	return veWordMatches;
}
