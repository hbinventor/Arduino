/*
 * AsyncWebserverEx.h
 *
 *  Created on: 12 thg 6, 2017
 *      Author: MyPC
 */

#ifndef BWEBSERVER_H_
#define BWEBSERVER_H_
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <AsyncJson.h>
#include <AsyncEventSource.h>
#include "Singleton.h"
#include "Session.h"
#include "Configs.h"
#include "Commander.h"
#include <TimeLib.h>

#include "DB.h"
 // HTTP

#define BROWSER_CACHE_DAYS	864000 // cache for 10 days
#define HTTP_PATH_ROOT	"/"
 // Params
#define PAR_COMMAND	"c"
#define PAR_STATUS	"status"
#define PAR_USERNAME	"username"
#define PAR_PASSWORD	"password"

#define PATH_ROOT	"/"
#define PATH_LOGIN	"/login.html"
#define PATH_LOGOUT	"/logout.html"
#define PATH_AJAX	"/ajax.html"

#define STT_OK	"ok"
#define STT_WRONG_PASS	"wrongPass"
#define STT_WRONG_USER	"wrongUser"

 // header
#define HEADER_CONNECTION	"Connection"
#define HEADER_SET_COOKIE	"Set-Cookie"

class BWebServer:
		public Singleton<BWebServer>,
		public AsyncWebServer,
		public Session
{
public:
	BWebServer();
	void start();
	void init();
	USER_ROLE getClientRole(AsyncWebServerRequest * request);

	void handleNotFound(AsyncWebServerRequest *request);
	void handleLogin(AsyncWebServerRequest *request);
	void handleLogout(AsyncWebServerRequest *request);
	void handleAjax(AsyncWebServerRequest * request);
	void handleBody(AsyncWebServerRequest  *request, uint8_t *data, size_t len, size_t index, size_t total);
	void sendFile(AsyncWebServerRequest *request, const char * filename, const char *content_type = (const char *) 0, bool isGzip = false, USER_ROLE role=NONE, unsigned long expire = 0UL);
	void sendStaticFile(AsyncWebServerRequest *request, const char * filename, const char *content_type = (const char *) 0, bool isGzip = true, USER_ROLE role=NONE);
	void sendGzipFile(AsyncWebServerRequest *request, const char * filename,  const char *content_type = (const char *) 0, USER_ROLE role=NONE, unsigned long expire = 0UL);

protected:
	virtual ~BWebServer();
	String _body;
	int _getBody;

	String getClientCookie(AsyncWebServerRequest * request);
	String newCookieString(USER_ROLE role);
};

#endif /* BWEBSERVER_H_ */
