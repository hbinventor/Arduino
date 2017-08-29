// 
// 
// 

#include "BWifi.h"

bool BWifi::addAPToList(const char * ssid, int rssi)
{
	BWifiInfo	wf;
	wf.rssi = rssi;
	strncpy(wf.ssid, ssid, 31);
	insertAPToList(wf);
	return true;
}

bool BWifi::insertAPToList(BWifiInfo & wifi)
{
	WifiList.push_back(wifi);
	return true;
}

int BWifi::getRelativeFromRssi(int rssi)
{
	int quality = 0;
	if (rssi <= -100) {
		quality = 0;
	}
	else if (rssi >= -50) {
		quality = 100;
	}
	else {
		quality = 2 * (rssi + 100);
	}
	return quality;
}

void BWifi::init()
{
	
}

bool BWifi::scanToList()
{
	if (getMode() != WIFI_AP_STA || getMode() != WIFI_AP)
	{
		mode(WIFI_AP_STA);
	}
	disconnect();
	delay(500);
	DBG("Start WiFi scanning...");
	int n = scanNetworks();
	DBG("Scan done.");
	WifiList.clear();
	if (n <= 0)
	{
		DBG("No WiFi network found.");
		return false;
	}
	for (int i = 0; i < n; i++)
	{
		yield();
		if (SSID(i).indexOf('"') < 0)
		{
			int quality = getRelativeFromRssi(RSSI(i));
			if (quality)
				addAPToList(SSID(i).c_str(), quality);
			DBG2F0("ssid ", i);
			DBG2F0(" : ", SSID(i));
			DBG2F(" - quality ", quality);
		}
	}
	return true;
}

bool BWifi::hasAP(String ssid)
{
	int totalAP = WifiList.size();
	for (int i = 0; i < totalAP; i++)
	{
		if (ssid.equals(WifiList[i].ssid))
		{
			return true;
		}
	}
	return false;
}

bool BWifi::printWifiListTo(String & buffer)
{
	int totalAP = WifiList.size();
	StaticJsonBuffer<300> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	for (int i = 0; i < totalAP; i++) {
		yield();
		if (WifiList[i].ssid) {
			String key = WifiList[i].ssid;
			key += " (";
			key += WifiList[i].rssi;
			key += "%)";
			root.set(key, WifiList[i].ssid);
		}
	}
	root.printTo(buffer);
	return true;
}

