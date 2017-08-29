/*
 * Session.h
 *
 *  Created on: 26 thg 8, 2017
 *      Author: MyPC
 */

#ifndef SESSION_H_
#define SESSION_H_

#include <Arduino.h>
#include <TimeLib.h>
#include "Configs.h"
#include <vector>
// SESSION
#define MAX_SESSION  10
#define SESSION_ID_LENGTH 33
#define SESSION_EXPIRE_TIME   (600) //  seconds


typedef struct __attribute__((packed)) SessionParam_t{
	char name[SESSION_ID_LENGTH];
	int role;
	time_t time;
} SessionParam;

class Session{
private:
	std::vector<SessionParam> _SESSION;
public:
	Session();
	virtual ~Session();
	String create(int role);
	void remove(String session);
	int getRole(String session);
	int setRole(String session, int role);
	static String randomString();
	void checkExpired();
};

#endif /* SESSION_H_ */
