/*
 * TimeService.cpp
 *
 *  Created on: Aug 16, 2017
 *      Author: binhpham
 */

#include "TimeService.h"

int TimeService::TimeZoneID = 30;

TimeService::TimeService() {
	// TODO Auto-generated constructor stub
	ntpServerName = "pool.ntp.org";
	packetBuffer = (byte*)malloc(NTP_PACKET_SIZE);
	lastSync = 0;
	resyncPeriod = 0;
}

TimeService::~TimeService() {
	// TODO Auto-generated destructor stub
	if ( packetBuffer ) free(packetBuffer);
}

void TimeService::setNTPServer(const char* server) {
	if ( server==NULL || !server[0]) return;
	ntpServerName = server;
}

bool TimeService::AutoSync(time_t ifSyncOK, time_t ifSyncFailed) {
	if ( now() > resyncPeriod ) {
		if ( Sync() ) {
			resyncPeriod = now() + ifSyncOK;
		}
		else
		{
			resyncPeriod = now() + ifSyncFailed;
			return false;
		}
	}
	return true;
}

bool TimeService::Sync() {
	AsyncUDP udp;

	if ( !udp.listen(NTP_LOCAL_PORT) ) {
		DBG("Cannot listen for UDP");
		return false;
	}
	udp.onPacket([&](AsyncUDPPacket packet) {
		if (packet.length() >= NTP_PACKET_SIZE) {
			DBG("-- Received NTP Response");
			DBG2F("packet received, length=",packet.length());
			//udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
			//break;
			if (packet.length() < NTP_PACKET_SIZE) {
				DBG("no packet yet");
				return false;
			}
			packetBuffer = packet.data();
			//the timestamp starts at byte 40 of the received packet and is four bytes,
			// or two words, long. First, esxtract the two words:
			unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
			unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
			// combine the four bytes (two words) into a long integer
			// this is NTP time (seconds since Jan 1 1900):
			unsigned long secsSince1900 = highWord << 16 | lowWord;
			DBG2F("Seconds since Jan 1 1900 = ", secsSince1900);

			// now convert NTP time into everyday time:
			// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
			const unsigned long seventyYears = 2208988800UL;
			// subtract seventy years:
			lastSync = secsSince1900 - seventyYears;
			setTime(lastSync);
			// print Unix time:
			DBG2F("Unix time = ",lastSync);
		}
		udp.close();
		return true;
	});
	if ( !sendNTPpacket(udp) ) {
		udp.close();
		DBG("Cannot send NTP packet");
		return false;
	}
}

bool TimeService::sendNTPpacket(AsyncUDP& udp) {
	if ( !packetBuffer ) {
		DBG("Error: Cannot allocate memory");
		return false;
	}
	// resolve NTP server name.
	IPAddress address;
	if ( WiFi.hostByName(ntpServerName.c_str(), address)!=1 ) {
		DBG2F("Cannot resolve host: ",ntpServerName);
		return false;
	}

	DBG2F("sending NTP packet...",ntpServerName);
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp.connect(address, 123); //NTP requests are to port 123
	udp.write(packetBuffer, NTP_PACKET_SIZE);
	//udp.endPacket();
	return true;
}
bool TimeService::setTimeZone(int zoneID) {
	if ( zoneID<0 || zoneID>=MAX_TIMEZONE_TABLE ) return false;
	TimeZoneID = zoneID;
	return true;
}

bool TimeService::getTimeZoneRule(int idx) {
	long *p = (long*)zr;
	long *q = (long*)&TimeZoneRule[idx*2];
	int i;
	int n = sizeof(TimeChangeRule)/2;
	if ( idx>=MAX_TIMEZONE_TABLE ) return false;
	for (i=0; i<n; i++) {
		yield();
		*p = *q;
		p++;
		q++;
	}
	return true;
}

time_t TimeService::getLocalTime(int timeZoneIndex) {
	if ( timeZoneIndex<0 || timeZoneIndex>=MAX_TIMEZONE_TABLE ) return 0;
	getTimeZoneRule(timeZoneIndex);

	Timezone tz(zr[0],zr[1]);
	time_t t = tz.toLocal(now());

	return t;
}

void TimeService::toJsonString(String& result) {
	StaticJsonBuffer<150> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["server"] = ntpServerName;
	root["time"] = (unsigned long)(getLocalTime(TimeZoneID));
	root["zone"] = TimeZoneID;
	root.printTo(result);
}

