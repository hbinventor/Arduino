/*
 * EmailSMSWifi.cpp
 *
 *  Created on: Aug 15, 2017
 *      Author: binhpham
 */

#include "BWebAPI.h"

//const char* EmailSMSWifi::host = "api.elasticemail.com";
//const char* EmailSMSWifi::sendEmailPath = "/v2/email/send?";
//const char* EmailSMSWifi::sendSmsPath = "/v2/sms/send?";

BWebAPI::BWebAPI() {
	// TODO Auto-generated constructor stub

}

BWebAPI::~BWebAPI() {
	// TODO Auto-generated destructor stub
}

bool BWebAPI::sendEmail(const char* from, const char* to,
		const char* subject, const char* body) {
	DBG2F("Heap size: ",ESP.getFreeHeap());
	HTTPClient http;
	String url;
	url.reserve(150);
	url = "http://";
//	url += host;
//	url += path;
	addParam(url, "apikey","api", true);//DB::instance()->GetApiKey().c_str()
	addParam(url, "from", from);
	addParam(url, "sender", from);
	addParam(url, "replyTo", from);
	addParam(url, "subject", subject);
	addParam(url, "to", to);
	addParam(url, "bodyText", body);
	ESP.wdtDisable();
	DBG2F("Open URL: ", url);
	if ( !http.begin(url) ) {
		DBG2F("Error opening URL: ",url);
		ESP.wdtEnable(10000);
		return false;
	}
	int httpCode = http.GET();
	DBG2F("HTTP Code: ",httpCode);
	bool bRvl = false;
	if (httpCode == HTTP_CODE_OK) {
		String payload = http.getString();
		DBG(payload);
		if (payload.indexOf("{\"success\":true")>=0) bRvl = true;
	}
	http.end();
	ESP.wdtEnable(10000);
	return bRvl;
}

bool BWebAPI::sendSms(const char* to, const char* body) {
	if ( !to || !body ) return false;
	DBG2F("Heap size: ",ESP.getFreeHeap());
	HTTPClient http;
	String url;
	url.reserve(150);
	url = "http://";
	//url += ModuleSettings::getInstance()->GetSMSServer();
	url +="?";
	addParam(url, "type","sms", true);	// first param
//	addParam(url, "sms_api_key",ModuleSettings::getInstance()->GetSMSApiKey().c_str());
//	addParam(url, "sms_api_secret",ModuleSettings::getInstance()->GetSMSApiSecret().c_str());
	addParam(url, "to", formatPhoneNumber(to).c_str());
	addParam(url, "text", body);

	ESP.wdtDisable();
	DBG2F("Open URL: ",url);
	if ( !http.begin(url) ) {
		DBG2F("Error opening URL: ",url);
		ESP.wdtEnable(10000);
		return false;
	}

	int httpCode = http.GET();
	DBG2F("HTTP Code: ",httpCode);
	bool bRvl = false;
	if (httpCode == HTTP_CODE_OK) {
		String payload = http.getString();
		DBG(payload);
		if (payload.indexOf("queued")>0) bRvl = true;
	}
	http.end();
	ESP.wdtEnable(10000);
	return bRvl;
}

void BWebAPI::addParam(String& uri, const char* paramName, const char* paramValue, bool isFirstParam) {
	static const char hex[] = "0123456789abcdef";
	char buf[4];
	unsigned int i, c;
	int n = strlen(paramValue);
	if (!isFirstParam ) uri += "&";
	uri += paramName;
	uri += "=";

	for (i=0; i<n; i++) {
		delay(0);
		c = paramValue[i];
		if( ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') ) {
			uri += (char)c;
		}
		else {
			buf[0] = '%';
			buf[1] = hex[(c>>4)&0xf];
			buf[2] = hex[c&0xf];
			buf[3] = 0;
			uri += buf;
		}
	}
}

String BWebAPI::getPublicIP() {
}

bool BWebAPI::setEmailServer(char* server) {
	strcpy(EmailServer, server);
}

bool BWebAPI::setEmailAPI(char* api) {
}

bool BWebAPI::setSMSServer(char* server) {
}

bool BWebAPI::setSMSAPI(char* api, char* apiSecret) {
}

String BWebAPI::formatPhoneNumber(const char* number) {
	String phone = "";
	if ( number==NULL ) return phone;
	int len = strlen(number);
	if ( len<10 ) return phone;
	phone.reserve(len+1);
	int i = 0;
	for (i=0; i<len; i++) {
	delay(0);
	if ( (number[i]>='0' && number[i]<='9') || number[i]=='+' ) phone += number[i];
	}
	if ( phone.charAt(0)=='+' ) return phone;
	len = phone.length();
	if ( len==10 ) return String("+1") + phone;
	if ( len>11 && phone.startsWith("00") ) return String("+") + phone.substring(2);
	return String("+") + phone;
}
