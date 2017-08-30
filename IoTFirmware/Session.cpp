/*
 * Session.cpp
 *
 *  Created on: 26 thg 8, 2017
 *      Author: MyPC
 */

#include "Session.h"

Session::Session() {
	// TODO Auto-generated constructor stub

}

Session::~Session() {
	// TODO Auto-generated destructor stub
}

String Session::create(USER_ROLE role) {
	if(_SESSION.size() >= MAX_SESSION)	return String("");
	SessionParam newSS;
	strcpy(newSS.name, randomString().c_str());
	newSS.expired = now() + SESSION_EXPIRE_TIME;
	newSS.role = role;
	_SESSION.push_back(newSS);
	return newSS.name;
}

void Session::remove(String session) {
	int size = _SESSION.size();
	for(int i=0; i<size; i++){
		if(session.equalsIgnoreCase(_SESSION[i].name)){
			_SESSION.erase(_SESSION.begin() + i);
		}
	}
}


USER_ROLE Session::getRole(String session) {
	int size = _SESSION.size();
	if(size <=0 ||(session.length()<32)) return NONE;
	checkExpired();
	USER_ROLE retRole = NONE;
	for(int i=0; i<size; i++){
		if(session.equalsIgnoreCase(_SESSION[i].name)){
			_SESSION[i].expired = now() + SESSION_EXPIRE_TIME;
			retRole = _SESSION[i].role;
			return retRole;
		}
	}
	return retRole;
}

USER_ROLE Session::setRole(String session, USER_ROLE role) {
	int size = _SESSION.size();
	for(int i=0; i<size; i++){
		if(session.equalsIgnoreCase(_SESSION[i].name)){
			_SESSION[i].role = role;
			_SESSION[i].expired = now() + SESSION_EXPIRE_TIME;
			return _SESSION[i].role;
		}
	}
	return NONE;
}

void Session::checkExpired() {
	int size = _SESSION.size();
	if(size<=0){
		DBGF("No has Session");
		return;
	}else {
		time_t cur = now();
		for(int i=0; i<size; i++){
			DBG2F0("Session ", i);
			DBG2F("==> ", _SESSION[i].name);
			if(cur > _SESSION[i].expired){
				DBG2F("Delete Session expired: ", i);
				_SESSION.erase(_SESSION.begin() + i);
			}
		}
	}
}
String Session::randomString() {
	MD5Builder md5;
	md5.begin();
	md5.add(String(random(0x7fffffff)));
	md5.add(String(millis()));
	md5.calculate();
	return md5.toString();
}
