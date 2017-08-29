/*
 * EmailSMSWifi.h
 *
 *  Created on: Aug 15, 2017
 *      Author: binhpham
 *      Class gui Email va SMS thong qua server tra phi tren Internet
 */

#ifndef BWEB_API_H_
#define BWEB_API_H_

#include "Singleton.h"
#include "Configs.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DB.h"

class BWebAPI: public Singleton<BWebAPI> {
public:
	BWebAPI();
	virtual ~BWebAPI();
	bool sendEmail(const char* from, const char* to, const char* subject, const char* body);
	bool sendSms(const char* to, const char* body);
	String getPublicIP();
	bool setEmailServer(char* server);
	bool setEmailAPI(char* api);
	bool setSMSServer(char* server);
	bool setSMSAPI(char* api, char* apiSecret);
protected:
	char* EmailServer;
	const char* SMSServer;
	const char* IPServer;
	const char* EmailAPI;
	const char* SMSAPI;
	const char* SMSAPISecret;
	void addParam(String& uri, const char *paramName, const char *paramValue, bool isFirstParam=false);
	String formatPhoneNumber(const char* number);
};

#endif /* BWEB_API_H_ */
