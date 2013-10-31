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
		uint8_t m_outPin;
		int m_SSG;
		int m_FSG;
		int m_OFS;
}

#endif
