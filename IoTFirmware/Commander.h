/*
 * Commander.h
 *
 *  Created on: 4 thg 2, 2017
 *      Author: MyPC
 */

#ifndef COMMANDER_H_
#define COMMANDER_H_

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Configs.h"
#include "Singleton.h"
#include "DB.h"
#include "GPIOs.h"
#include "BMQTT.h"
#include "EventHandle.h"

#define COMM_COM	"cmd"

typedef enum { MQTT, HTTP } CmdProvider_t;

class Commander : public Singleton<Commander> {
	public:
		Commander();
		virtual ~Commander();
		static String processWebsever(const char * commandJson);
		static bool processMQTT(String topic, const char* payload);
		void testEvent(char* topic, char* payload);
	private:
};

#endif /* COMMANDER_H_ */
