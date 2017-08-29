// GPIOManager.h

#ifndef _GPIOMANAGER_h
#define _GPIOMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include "DB.h"
#include "Singleton.h"

typedef struct __attribute__((packed)) {
	int blinkTimes;	// so lan nhap nhay con lai, = -1: nhap nhay lien tuc
	unsigned long nextOn;	// next time will turn on
	unsigned long nextOff;	// next time will turn off
} OutputVar_t;

class GPIOs:public Singleton<GPIOs>
{
 protected:
	 OutputVar_t outputs[OUTPUT_PINS];
 public:
	 GPIOs() {};
	 virtual ~GPIOs() {};
	 void checkUpdate();
	 void loop();
	 void blink(int index, int times);
	 bool setValue(int index, int value);
	 bool getValue(int index);
	 void clearBlink(int index);
};


#endif

