/*
 * TimeService.h
 *
 *  Created on: Aug 16, 2017
 *      Author: binhpham
 */

#ifndef TIMESERVICE_H_
#define TIMESERVICE_H_
#include "Singleton.h"
#include "Configs.h"
#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <ArduinoJson.h>
#include "DB.h"

#define NTP_LOCAL_PORT 2395      // local port to listen for UDP packets
#define NTP_PACKET_SIZE 48 // NTP time stamp is in the first 48 bytes of the message
#define	MAX_TIMEZONE_TABLE	79

static const TimeChangeRule ICACHE_RODATA_ATTR TimeZoneRule[MAX_TIMEZONE_TABLE*2] {
	{"IDLW", First, Sun, Nov, 2, -720}, {"IDLW", First, Sun, Nov, 2, -720},
	{"CUT11", First, Sun, Nov, 2, -660},{"CUT11", First, Sun, Nov, 2, -660},
	{"HAST", First, Sun, Nov, 2, -600}, {"HAST", First, Sun, Nov, 2, -600},
	{"AKDT", Second, Sun, Mar, 2, -480},{"AKST", First, Sun, Nov, 2, -540},
	{"PDT", Second, Sun, Mar, 2, -420}, {"PST", First, Sun, Nov, 2, -480},
	{"PDT", Second, Sun, Mar, 2, -420}, {"PST", First, Sun, Nov, 2, -480},
	{"MDT", Second, Sun, Mar, 2, -360}, {"MST", First, Sun, Nov, 2, -420},
	{"MDT", Second, Sun, Mar, 2, -360}, {"MST", First, Sun, Nov, 2, -420},
	{"MST", First, Sun, Nov, 2, -420},  {"MST", First, Sun, Nov, 2, -420},
	{"CDT", Second, Sun, Mar, 2, -300}, {"CST", First, Sun, Nov, 2, -360},
	{"CST", First, Sun, Nov, 2, -360},  {"CST", First, Sun, Nov, 2, -360},
	{"CDT", Second, Sun, Mar, 2, -300}, {"CST", First, Sun, Nov, 2, -360},
	{"CST", First, Sun, Nov, 2, -360},  {"CST", First, Sun, Nov, 2, -360},
	{"EDT", Second, Sun, Mar, 2, -240}, {"EST", First, Sun, Nov, 2, -300},
	{"EDT", Second, Sun, Mar, 2, -240}, {"EST", First, Sun, Nov, 2, -300},
	{"ACT", First, Sun, Nov, 2, -300},  {"ACT", First, Sun, Nov, 2, -300},
	{"ADT", Second, Sun, Mar, 2, -240}, {"AST", First, Sun, Nov, 2, -180},
	{"VET", First, Sun, Nov, 2, -270},  {"VET", First, Sun, Nov, 2, -270},
	{"CLT", Second, Sun, May, 0, -240}, {"CLST", Second, Sun, Aug, 0, -180},
	{"PYT", Fourth, Sun, Mar, 0, -240}, {"PYST", First, Sun, Oct, 0, -180},
	{"AMT", Third, Sun, Feb, 0, -240},  {"AMST", Third, Sun, Oct, 0, -180},
	{"GYT", First, Sun, Nov, 2, -240},  {"GYT", First, Sun, Nov, 2, -240},
	{"NDT", Second, Sun, Mar, 2, -150}, {"NST", First, Sun, Nov, 2, -210},
	{"BRT", Third, Sun, Feb, 0, -180},  {"BRST", Third, Sun, Oct, 0, -120},
	{"ART", First, Sun, Nov, 2, -180},  {"ART", First, Sun, Nov, 2, -180},
	{"WGT", Fourth, Sat, Mar, 22, -180},{"WGST", Fourth, Sat, Oct, 23, -120},
	{"MALT", First, Sun, Nov, 2, -120}, {"MALT", First, Sun, Nov, 2, -120},
	{"AZOST", Last, Sun, Mar, 0, 0},  {"AZOT", Last, Sun, Oct, 1, -60},
	{"CVE", First, Sun, Nov, 2, -60}, {"CVE", First, Sun, Nov, 2, -60},
	{"BST", Last, Sun, Mar, 1, 60},   {"GMT", Last, Sun, Oct, 2, 0},
	{"CEST", Last, Sun, Mar, 2, 120}, {"CET", Last, Sun, Oct, 3, 60},
	{"CEST", Last, Sun, Mar, 2, 120}, {"CET", Last, Sun, Oct, 3, 60},
	{"CEST", Last, Sun, Mar, 2, 120}, {"CET", Last, Sun, Oct, 3, 60},
	{"CEST", Last, Sun, Mar, 2, 120}, {"CET", Last, Sun, Oct, 3, 60},
	{"WAT", First, Sun, Nov, 2, 60},  {"WAT", First, Sun, Nov, 2, 60},
	{"EEST", Last, Sun, Mar, 3, 180}, {"EET", Last, Sun, Mar, 4, 120},
	{"EET", Last, Sun, Mar, 2, 120},  {"EET", Last, Sun, Mar, 2, 120},
	{"EEST", Last, Sun, Mar, 3, 180}, {"EET", Last, Sun, Mar, 4, 120},
	{"EEST", Last, Sun, Mar, 3, 180}, {"EET", Last, Sun, Mar, 4, 120},
	{"EEST", Last, Sun, Mar, 3, 180}, {"EET", Last, Sun, Mar, 4, 120},
	{"EEST", Last, Sun, Mar, 3, 180}, {"EET", Last, Sun, Mar, 4, 120},
	{"MSK", First, Sun, Nov, 2, 120}, {"MSK", First, Sun, Nov, 2, 120},
	{"IDT", Fourth, Fri, Mar, 2, 180},  {"IST", Last, Sun, Oct, 2, 120},
	{"CAT", First, Sun, Nov, 2, 120}, {"CAT", First, Sun, Nov, 2, 120},
	{"MSK", First, Sun, Nov, 2, 180}, {"MSK", First, Sun, Nov, 2, 180},
	{"AST", First, Sun, Nov, 2, 180}, {"AST", First, Sun, Nov, 2, 180},
	{"EAT", First, Sun, Nov, 2, 180}, {"EAT", First, Sun, Nov, 2, 180},
	{"AST", First, Sun, Nov, 2, 180}, {"AST", First, Sun, Nov, 2, 180},
	{"GST", First, Sun, Nov, 2, 240}, {"GST", First, Sun, Nov, 2, 240},
	{"AZT", First, Sun, Nov, 2, 240}, {"AZT", First, Sun, Nov, 2, 240},
	{"AFT", First, Sun, Nov, 2, 270}, {"AFT", First, Sun, Nov, 2, 270},
	{"YEKT", First, Sun, Nov, 2, 300},  {"YEKT", First, Sun, Nov, 2, 300},
	{"PKT", First, Sun, Nov, 2, 300}, {"PKT", First, Sun, Nov, 2, 300},
	{"IST", First, Sun, Nov, 2, 330}, {"IST", First, Sun, Nov, 2, 330},
	{"NPT", First, Sun, Nov, 2, 345}, {"NPT", First, Sun, Nov, 2, 345},
	{"BST", First, Sun, Nov, 2, 360}, {"BST", First, Sun, Nov, 2, 360},
	{"IST", First, Sun, Nov, 2, 360}, {"IST", First, Sun, Nov, 2, 360},
	{"ALMT", First, Sun, Nov, 2, 360},  {"ALMT", First, Sun, Nov, 2, 360},
	{"MMT", First, Sun, Nov, 2, 390}, {"MMT", First, Sun, Nov, 2, 390},
	{"ICT", First, Sun, Nov, 2, 420}, {"ICT", First, Sun, Nov, 2, 420},
	{"KRAT", First, Sun, Nov, 2, 420},  {"KRAT", First, Sun, Nov, 2, 420},
	{"CST", First, Sun, Nov, 2, 480}, {"CST", First, Sun, Nov, 2, 480},
	{"SGT", First, Sun, Nov, 2, 480}, {"SGT", First, Sun, Nov, 2, 480},
	{"CST", First, Sun, Nov, 2, 480}, {"CST", First, Sun, Nov, 2, 480},
	{"AWST", First, Sun, Nov, 2, 480},  {"AWST", First, Sun, Nov, 2, 480},
	{"ULAST", Last, Sat, Mar, 2, 540},  {"ULAT", Last, Sat, Sep, 0, 480},
	{"KST", First, Sun, Nov, 2, 540}, {"KST", First, Sun, Nov, 2, 540},
	{"JST", First, Sun, Nov, 2, 540}, {"JST", First, Sun, Nov, 2, 540},
	{"YAKT", First, Sun, Nov, 2, 540},  {"YAKT", First, Sun, Nov, 2, 540},
	{"ACST", First, Sun, Nov, 2, 570},  {"ACST", First, Sun, Nov, 2, 570},
	{"ACDT", First, Sun, Oct, 2, 630},  {"ACST", First, Sun, Apr, 3, 570},
	{"AEDT", First, Sun, Oct, 2, 660},  {"AEST", First, Sun, Apr, 3, 600},
	{"AEST", First, Sun, Nov, 2, 600},  {"AEST", First, Sun, Nov, 2, 600},
	{"AEDT", First, Sun, Oct, 2, 660},  {"AEST", First, Sun, Apr, 3, 600},
	{"VLAT", First, Sun, Nov, 2, 600},  {"VLAT", First, Sun, Nov, 2, 600},
	{"CHST", First, Sun, Nov, 2, 600},  {"CHST", First, Sun, Nov, 2, 600},
	{"NCT", First, Sun, Nov, 2, 660}, {"NCT", First, Sun, Nov, 2, 660},
	{"MHT", First, Sun, Nov, 2, 720}, {"MHT", First, Sun, Nov, 2, 720},
	{"NZDT", Last, Sun, Sep, 2, 780}, {"NZST", First, Sun, Apr, 3, 720}
};

class TimeService:public Singleton<TimeService> {
public:
	TimeService();
	virtual ~TimeService();
	time_t static getLocalTime(int timeZoneIndex);
	void toJsonString(String& result);
	bool Sync();
	bool AutoSync(time_t ifSyncOK = 14400, time_t ifSyncFailed = 300);
protected:
	byte* packetBuffer;
	time_t lastSync;
	time_t resyncPeriod;

	static TimeChangeRule zr[2];
	bool static getTimeZoneRule(int idx);
	bool sendNTPpacket(AsyncUDP& udp);
};

#endif /* TIMESERVICE_H_ */
