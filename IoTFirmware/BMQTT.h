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
class BMQTT: public AsyncMqttClient
{
protected:
	static	BMQTT*	m_instance;
	void onWifiConnect(const WiFiEventStationModeGotIP & event);
public:
	BMQTT();
	virtual ~BMQTT();
	static BMQTT *getInstance() {
		if (m_instance == nullptr)
			m_instance = new BMQTT();
		return m_instance;
	}
	void init();
	void putGPIOStatus(int index, int value);
	void putSensorValue(String type, String value);
	void listen();	// Lang nghe topic cua ban than thiet bi
};

#endif

