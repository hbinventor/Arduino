// OTAUpdater.h

#ifndef _OTAUPDATER_h
#define _OTAUPDATER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Configs.h"
#include "BWifi.h"
#include <ESP8266httpUpdate.h>
#include <Ticker.h>
#define OTA_UPDATE_FREQ	30	// seconds
class OTAUpdater
{
 protected:
	 static t_httpUpdate_return UpdateResult;
	 static Ticker updateChecker;
 public:
	 static void checkUpdate();
	 static void init();
};


#endif

