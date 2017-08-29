// BMQTT.h

#ifndef _BMQTT_h
#define _BMQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <ESP8266WiFi.h>
#include <AsyncMqttClient.h>
#include "BWifi.h"
#include "DB.h"

#define TOPIC_PRIVATE_PREFIX	"private"
class BMQTT
		:public Singleton<BMQTT>,
		 public AsyncMqttClient
{
protected:
	void onWifiConnect(const WiFiEventStationModeGotIP & event);
public:
	BMQTT();
	virtual ~BMQTT();
	void init();
	void putGPIOStatus(int index, int value);
	void putSensorValue(String type, String value);
	void listen();	// Lang nghe topic cua ban than thiet bi
};

#endif

