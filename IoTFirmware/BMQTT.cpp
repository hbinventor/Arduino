// 
// 
// 

#include "BMQTT.h"

void BMQTT::init()
{


}

void BMQTT::putGPIOStatus(int index, int value)
{
	String meTopic = TOPIC_PRIVATE_PREFIX;
	meTopic += "/";
	meTopic += DB::instance()->getDeviceSerial();
	meTopic += "/status/";
	meTopic += index;
	if (value)
	{
		publish(meTopic.c_str(), 0, true, JsonTrueValue, sizeof(JsonTrueValue));
	}
	else
	{
		publish(meTopic.c_str(), 0, true, JsonFalseValue, sizeof(JsonFalseValue));
	}
}

void BMQTT::listen()
{
}


BMQTT BMQTT;

void BMQTT::onWifiConnect(const WiFiEventStationModeGotIP& event) {
	DBGF("Connecting to MQTT...");
	connect();
}

BMQTT::BMQTT() {
	BWifi::instance()->onStationModeGotIP(std::bind(&BMQTT::onWifiConnect, this, std::placeholders::_1));
}

BMQTT::~BMQTT() {
}

void BMQTT::putSensorValue(String type, String value) {
}
