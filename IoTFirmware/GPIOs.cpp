// 
// 
// 

#include "GPIOs.h"

void GPIOs::checkUpdate()
{
	for (int i = 0; i < OUTPUT_PINS; i++)
	{
		yield();
		pinMode(DB::instance()->getOutputPin(i), OUTPUT);
		if (DB::instance()->getOutputPin(i)!= DB::instance()->getOutputFeedback(i))
		{
			pinMode(DB::instance()->getOutputFeedback(i), INPUT);
		}
		clearBlink(i);
	}
}

void GPIOs::loop()
{
	for (int i = 0; i < OUTPUT_PINS; i++)
	{
		yield();
		unsigned long timeNow = millis();
		if (DB::instance()->getOutputType(i)==BLINK)
		{
			if (outputs[i].blinkTimes > 0 || outputs[i].blinkTimes == -1)
			{
				if (outputs[i].nextOn <= timeNow)
				{
					digitalWrite(DB::instance()->getOutputPin(i), HIGH);
					outputs[i].nextOn = timeNow + DB::instance()->getOutputTimeOn(i) + DB::instance()->getOutputTimeOff(i);
				}

				if (outputs[i].nextOff <= timeNow)
				{
					digitalWrite(DB::instance()->getOutputPin(i), LOW);
					outputs[i].nextOff = timeNow + DB::instance()->getOutputTimeOff(i) + DB::instance()->getOutputTimeOn(i);
					if (outputs[i].blinkTimes != -1)
					{
						outputs[i].blinkTimes--;
					}
				}

			}
		}
	}
}

void GPIOs::blink(int index, int times)
{
	if (index >= OUTPUT_PINS || index<0)
	{
		return;
	}
	unsigned long timeNow = millis();
	outputs[index].blinkTimes = times;
	outputs[index].nextOn = timeNow;
	outputs[index].nextOff = timeNow + DB::instance()->getOutputTimeOn(index);
}

bool GPIOs::setValue(int index, int value)
{
	if (index >= OUTPUT_PINS || index<0)
	{
		return false;
	}
	if (DB::instance()->getOutputType(index) == TOGGLE)
	{
		//DBG2F0("Setpin: ", DeviceSetting::getInstance()->getOutputPin(index));
		//DBG2F("Setpin: ", value);
		if (DB::instance()->getOutputPin(index)>=0)
		{
			digitalWrite(DB::instance()->getOutputPin(index), value);
			return true;
		}
	}
	return false;
}

bool GPIOs::getValue(int index)
{
	if (index >= OUTPUT_PINS || index<0)
	{
		return false;
	}
	//DBG2F0("Getpin: ", DeviceSetting::getInstance()->getOutputFeedback(index));
	return digitalRead(DB::instance()->getOutputFeedback(index));
}

void GPIOs::clearBlink(int index)
{
	if (index >= OUTPUT_PINS || index<0)
	{
		return;
	}
	OutputVar_t output;
	output.blinkTimes = 0;
	output.nextOn = millis();
	output.nextOff = millis();
	outputs[index] = output;
}

