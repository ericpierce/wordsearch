CC = c++
CFLAGS = -Wall -g
FLAGS = -lboost_regex
LDFLAGS = -lm

all : wordsearch

LetterMonitor.o : LetterMonitor.cpp LetterMonitor.h
	${CC} ${CFLAGS} -c LetterMonitor.cpp

WordService.o : WordService.cpp WordService.h
	${CC} ${CFLAGS} -c WordService.cpp

WordUtil.o : WordUtil.cpp WordUtil.h
	${CC} ${CFLAGS} -c WordUtil.cpp

wordsearch.o : wordsearch.cpp WordUtil.h
	${CC} ${CFLAGS} -c wordsearch.cpp

wordsearch : wordsearch.o WordUtil.o WordService.o LetterMonitor.o
	${CC} ${CFLAGS} WordUtil.o WordService.o LetterMonitor.o wordsearch.o ${LDFLAGS} ${FLAGS} -o wordsearch

clean : 
	rm -rf *o wordsearch
