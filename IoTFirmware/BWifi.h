// BWifi.h

#ifndef _BWIFI_h
#define _BWIFI_h

#include <vector>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiGeneric.h>
#include <ArduinoJson.h>
#include "Singleton.h"
#include "Configs.h"
#include "BMQTT.h"

#define MAX_LIST_AP	20
#define WIFI_CONNECT_TIMEOUT	30
#define WIFI_RECONNECT_TIME	10000

typedef struct BWifiInfo_t{
	char ssid[32];
	int rssi;
} BWifiInfo;

class BWifi
		:public Singleton<BWifi>,
		 public ESP8266WiFiClass
{
 protected:
	std::vector<BWifiInfo> WifiList;
	bool addAPToList(const char* ssid, int rssi);
	bool insertAPToList(BWifiInfo& wifi);
	int getRelativeFromRssi(int rssi);
 public:
	void init();
	bool scanToList();
	bool hasAP(String ssid);
	bool printWifiListTo(String& buffer);
};

#endif

