/*
 * Commander.cpp
 *
 *  Created on: 4 thg 2, 2017
 *      Author: MyPC
 */

#include "Commander.h"

Commander::Commander() {
	// TODO Auto-generated constructor stub
}

Commander::~Commander() {
	// TODO Auto-generated destructor stub
}

String Commander::processWebsever(const char * commandJson)
{
	/*
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(commandJson);
	String cmd = root["cmd"].asString();
	DBG(root["cmd"].asString());
	if (cmd == "control") {
		DBG("control");
	}
	else if (cmd == "networkSetup") {
		DBG(root["dhcp"].asString());
		DBG(root["ssid"].asString());
		DBG(root["wifiPassword"].asString());
		DBG(root["port"].asString());
		DBG(root["_IP"].asString());
		DBG(root["_Mask"].asString());
		DBG(root["_Gw"].asString());
		bool dhcp = root["dhcp"];
		bool result;
		if (dhcp) {
			result = DeviceSetting::getInstance()->setSSID(root["ssid"].asString())&
				DeviceSetting::getInstance()->setWifiPassword(root["wifiPassword"].asString())&
				DeviceSetting::getInstance()->setHTTPPort(root["port"].as<int>())&
				DeviceSetting::getInstance()->setDHCP(dhcp);
		}
		else {
			result = DeviceSetting::getInstance()->setSSID(root["ssid"].asString())&
				DeviceSetting::getInstance()->setWifiPassword(root["wifiPassword"].asString())&
				DeviceSetting::getInstance()->setHTTPPort(root["port"].as<int>())&
				DeviceSetting::getInstance()->setIP(root["_IP"].asString())&
				DeviceSetting::getInstance()->setMask(root["_Mask"].asString())&
				DeviceSetting::getInstance()->setGateway(root["_Gw"].asString())&
				DeviceSetting::getInstance()->setDHCP(dhcp);
		}
		if (result) {
			DeviceSetting::getInstance()->saveSettings();
			return FPSTR(JsonTrue);
		}
		else return FPSTR(JsonFalse);
	}
	else if (cmd == "debug") {
		DBG("debug");
	}
	else {
		DBG("exit");
	}
	*/
	return JsonStatusFalse;
}

bool Commander::processMQTT(String topic, const char* payload)
{
	DynamicJsonBuffer jsonBuffer;
	JsonObject& root = jsonBuffer.parseObject(payload);
	/*
	for (JsonObject::iterator it = root.begin(); it != root.end(); ++it)
	{
		DBG2F("key: ", it->key);
		DBG2F("value: ", it->value.asString());
	}
	*/
	if (topic.indexOf(DB::instance()->getDeviceSerial())>0)
	{
		if (topic.endsWith("control"))
		{
			int pin = root["pin"];
			int val = root["value"];
			//DBG2F("pin: ", pin);
			//DBG2F("value: ", val);
			GPIOs::instance()->setValue(pin, val);
			//::putGPIOStatus(pin, GPIOs::instance()->getValue(pin));
			return true;
		}
		else if (topic.endsWith("setup"))
		{

		}
		else if (topic.endsWith("system"))
		{

		}
	}
	return false;
}
