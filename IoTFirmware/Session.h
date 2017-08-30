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

typedef enum USER_ROLE_t{
	NONE = 0,
	USER,
	ADMIN
} USER_ROLE;

typedef struct __attribute__((packed)) SessionParam_t{
	char name[SESSION_ID_LENGTH];
	time_t expired;
	USER_ROLE role;
} SessionParam;

class Session{
private:
	std::vector<SessionParam> _SESSION;
public:
	Session();
	virtual ~Session();
	String create(USER_ROLE role);
	void remove(String session);
	USER_ROLE getRole(String session);
	USER_ROLE setRole(String session, USER_ROLE role);
	static String randomString();
	void checkExpired();
};

#endif /* SESSION_H_ */
