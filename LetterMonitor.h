#ifndef LETTERMONITOR_H_
#define LETTERMONITOR_H_

class LetterMonitor {
	public:
		LetterMonitor();
		~LetterMonitor();
		void init();
		int getAvailable();
		int getFound();
		void setAvailable( int );
		void setFound( int );
	protected:
		int available;
		int found;
};

#endif /*LETTERMONITOR_H_*/ 
