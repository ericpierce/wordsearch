# wordsearch
This program helps fascilitate (&ast;cough&ast; cheat) the lookup of word possibilities in games like Wordfeud, WWF, Scrabble, etc.

## Dependencies
### boost-regex
Linux: install something like **libboost-regex1.xx-dev** from your distro's package manager.

## Compile and Run
`make && ./wordsearch`

## Usage
```
wordsearch v0.1

First, type *every* letter in your hand (e.g., if you have a letter twice, type it twice). Skip wild letters in this step. 
Letter Group #1: eegacl    <- We were dealt these 6 letters plus 1 wild letter. Enter them all (note: 2 Es) and skip the wild letter.

0 = wild letter
1 = instance of letter from group
[a-z] = any static letter
Word pattern: 111111       <- check for any 6 letter words.
No matches found.

Word pattern: 11111        <- check for any 5 letter words.

aglee

eagle

glace
Word pattern: b11111       <- check for any 6 letter words starting with a 'b'

beagle
Word pattern: 11011        <- check for any 5 letter words with our wild letter in the middle.

ackee

agile

aglee

agree

allee

cable

cadge

camel
...
```
