/*
 * Alarm.h
 *
 *  Created on: Aug 16, 2017
 *      Author: binhpham
 */

#ifndef ALARM_H_
#define ALARM_H_

#include <Arduino.h>
#include "Singleton.h"
#include "Configs.h"

class Alarm: public Singleton<Alarm> {
public:
	Alarm();
	virtual ~Alarm();
	void resetLogic();
	void disableBell();
	void runLogic();
	bool getOutputActive();
public:
	uint32_t _lastTimeLogic;
	bool _outputActive, _bellState, _waitingInput;
	int _inputPin, _bellPin;
};

#endif /* ALARM_H_ */
