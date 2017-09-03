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
	_wifiList.push_back(wifi);
	return true;
}

int BWifi::rssiToRelative(int rssi)
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
	if (WiFi.getMode() != WIFI_AP_STA || WiFi.getMode() != WIFI_AP)
	{
		WiFi.mode(WIFI_AP_STA);
	}
	WiFi.disconnect();
	delay(500);
	DBG("Start WiFi scanning...");
	int n = WiFi.scanNetworks();
	DBG("Scan done.");
	_wifiList.clear();
	if (n <= 0)
	{
		DBG("No WiFi network found.");
		return false;
	}
	for (int i = 0; i < n; i++)
	{
		yield();
		if (WiFi.SSID(i).indexOf('"') < 0)
		{
			int quality = rssiToRelative(WiFi.RSSI(i));
			if (quality)
				addAPToList(WiFi.SSID(i).c_str(), quality);
			DBG2F0("ssid ", i);
			DBG2F0(" : ", WiFi.SSID(i));
			DBG2F(" - quality ", quality);
		}
	}
	return true;
}

bool BWifi::hasAP(String ssid)
{
	int totalAP = _wifiList.size();
	for (int i = 0; i < totalAP; i++)
	{
		if (ssid.equals(_wifiList[i].ssid))
		{
			return true;
		}
	}
	return false;
}

bool BWifi::printWifiListTo(String & buffer)
{
	int totalAP = _wifiList.size();
	StaticJsonBuffer<300> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	for (int i = 0; i < totalAP; i++) {
		yield();
		if (_wifiList[i].ssid) {
			String key = _wifiList[i].ssid;
			key += " (";
			key += _wifiList[i].rssi;
			key += "%)";
			root.set(key, _wifiList[i].ssid);
		}
	}
	root.printTo(buffer);
	return true;
}

