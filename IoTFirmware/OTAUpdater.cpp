// 
// 
// 

#include "OTAUpdater.h"
t_httpUpdate_return OTAUpdater::UpdateResult;
Ticker OTAUpdater::updateChecker;

void OTAUpdater::checkUpdate()
{
	DBGF("OTA Updateing.");
	if (WiFi.status()==WL_CONNECTED)
	{
		UpdateResult = ESPhttpUpdate.update(HBOS_UPDATE_SERVER, 80, HBOS_UPDATE_PATH, HBOS_Version);
		switch (UpdateResult) {
		case HTTP_UPDATE_FAILED:
			DBGF("[update] Update failed.");
			break;
		case HTTP_UPDATE_NO_UPDATES:
			DBGF("[update] Update no Update.");
			break;
		case HTTP_UPDATE_OK:
			DBGF("[update] Update ok."); // may not called we reboot the ESP
			break;
		}
	}
}

void OTAUpdater::init()
{
	DBGF("Start timer OTA Update every 1 hour");
	updateChecker.attach(OTA_UPDATE_FREQ, checkUpdate); //kiem tra update moi 1h
}


