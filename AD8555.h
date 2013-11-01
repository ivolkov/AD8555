#ifndef Arduino_ad8555_h
#define Arduino_ad8555_h

class AD8555
{
	public:
		AD8555(int outputPin);
		bool setSecondStageGain(int value);
		bool setFirstStageGain(int value);
		bool setOffset(int value);
		bool simulate();
		bool program();
		
	private:
		void sendBit(bool value);
		void send0();
		void send1();
		uint8_t outPin;
		int SSG;
		int FSG;
		int OFS;
}

#endif
