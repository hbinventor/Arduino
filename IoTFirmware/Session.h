/*
 * Session.h
 *
 *  Created on: 26 thg 8, 2017
 *      Author: MyPC
 */

#ifndef SESSION_H_
#define SESSION_H_

#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include "Configs.h"
// SESSION
#define MAX_SESSION  10
#define SESSION_ID_LENGTH 33
#define SESSION_EXPIRE_TIME   (600) //  seconds


typedef struct __attribute__((packed)) {
	char name[33];
	int role;
	time_t time;
} SessionParam;

class Session{
public:
	Session();
	virtual ~Session();
	SessionParam *SS;
	int numSession;
	void startSession();
	String createSS(int role);
	void deleteSS(String session);
	int getSSUserRole(String session);
	int setSSUserRole(String session, int role);
	static String getRandomString();
	void checkSSExpireTime();
};

#endif /* SESSION_H_ */
