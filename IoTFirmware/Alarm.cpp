/*
 * Alarm.cpp
 *
 *  Created on: Aug 16, 2017
 *      Author: binhpham
 */

#include "Alarm.h"

Alarm::Alarm() {
	// TODO Auto-generated constructor stub
	_inputPin = ARM_INPUT_PIN;
	_bellPin = BELL_PIN;
	_waitingInput = true;
	_outputActive = true;
	_bellState = LOW;
	pinMode(_bellPin,OUTPUT);
	digitalWrite(_bellPin,_bellState);
	_lastTimeLogic = millis();
}

Alarm::~Alarm() {
	// TODO Auto-generated destructor stub
}

void Alarm::resetLogic() {
	_waitingInput = true;
	_outputActive = true;
	_bellState = LOW;
}

void Alarm::disableBell() {
	_outputActive = false;
}

void Alarm::runLogic() {
	/*
	if(_waitingInput&&(digitalRead(Constants::getInstance()->inputPins[0])||digitalRead(Constants::getInstance()->inputPins[1]))){
		_waitingInput = false;
		_bellState = HIGH;
		_lastTimeLogic = millis();
	}
	if(!_waitingInput){
	if(millis()-_lastTimeLogic >= ModuleSettings::getInstance()->Data.ArmLogicTime*60000UL){
		_lastTimeLogic = millis();
		_bellState = !_bellState;
	}
	}else{
		_bellState = LOW;
	}
	if(_outputActive) digitalWrite(_bellPin,_bellState);
	else digitalWrite(_bellPin,LOW);
	*/
}

bool Alarm::getOutputActive() {
	return _outputActive;
}
