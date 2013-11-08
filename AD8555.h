#ifndef AD8555_h
#define AD8555_h

#include "Arduino.h" 

#define FUNC_CHANGE_SENSE_CURRENT 0b00
#define FUNC_SIMULATE	0b01
#define FUNC_PROGRAM	0b10
#define FUNC_READ		0b11

#define	PAR_SSG_CODE	0b00
#define PAR_FSG_CODE	0b01
#define PAR_OFS_CODE	0b10
#define PAR_OTHER_FUNC	0b11

class AD8555
{
	public:
		AD8555(uint8_t outputPin);
		bool setSecondStageGain(uint8_t value);
		bool setFirstStageGain(uint8_t value);
		bool setOffset(uint8_t value);
		void simulate();
		void program();
		void sendFields(uint8_t mode, uint8_t func, uint8_t value);
		
	private:
		void blowSSGFuse(uint8_t Value);
		void blowFSGFuse(uint8_t Value);
		void blowOFSFuse(uint8_t Value);
		void blowMSF();
		void sendField0();
		void sendField1(uint8_t value);
		void sendField2(uint8_t value);
		void sendField3();
		void sendField4(uint8_t value);
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
