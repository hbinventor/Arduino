/*
 * BSerial.h
 *
 *  Created on: 25 thg 8, 2017
 *      Author: MyPC
 */

#ifndef BSERIAL_H_
#define BSERIAL_H_

#include <Arduino.h>
#include "Singleton.h"

class BSerial: public Singleton<BSerial> {
public:
	BSerial();
	virtual ~BSerial();
};

#endif /* BSERIAL_H_ */
