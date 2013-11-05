#ifndef AD8555_h
#define AD8555_h

#include "Arduino.h" 

class AD8555
{
	public:
		AD8555(int outputPin);
		bool setSecondStageGain(int value);
		bool setFirstStageGain(int value);
		bool setOffset(int value);
		void simulate();
		void program();
		
	private:
		void blowSSGFuse(unsigned char Value);
		void blowFSGFuse(unsigned char Value);
		void blowOFSFuse(unsigned char Value);
		void blowMSF();
		void sendField0();
		void sendField1(unsigned char value);
		void sendField2(unsigned char value);
		void sendField3();
		void sendField4(unsigned char value);
		void sendField5();
		void sendBit(bool value);
		void send0();
		void send1();
		int outPin;
		int SSG;
		int FSG;
		int OFS;
};

#endif
