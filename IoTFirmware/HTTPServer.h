/*
 * AsyncWebserverEx.h
 *
 *  Created on: 12 thg 6, 2017
 *      Author: MyPC
 */

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_
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
#include "html/favicon.png.h"
#include "html/jquery-3.1.0.min.js.gz.h"
#include "html/md5.min.js.gz.h"
#include "html/main.js.h"
#include "html/main.css.h"
#include "html/admin.html.h"
#include "html/device.html.h"
#include "html/email.html.h"
#include "html/general.html.h"
#include "html/gpio.html.h"
#include "html/login.html.h"
#include "html/network.html.h"
#include "html/settings.html.h"
#include "html/time.html.h"
#include "html/co22.png.h"
#include "html/fire1.png.h"
#include "html/hum2.png.h"
#include "html/temp1.png.h"
 // HTTP
#define BROWSER_CACHE_DAYS	864000 // cache for 10 days
#define HTTP_PATH_ROOT	"/"
#define HTTP_PATH_FAVICON	"/favicon.png"
#define HTTP_PATH_JQUERY	"/js/jquery-3.1.0.min.js.gz"
#define HTTP_PATH_MD5_JS	"/js/md5.min.js.gz"
#define HTTP_PATH_MAIN_JS	"/js/main.js"
#define HTTP_PATH_SKELETON_CSS	"/css/skeleton.css"
#define HTTP_PATH_NORMALIZE_CSS	"/css/normalize.css"
#define HTTP_PATH_MAIN_CSS	"/css/main.css"
#define HTTP_PINMAP_JS	"/js/ioPinMaps.js"
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

class HTTPServer:
		public Singleton<HTTPServer>,
		public AsyncWebServer,
		public Session
{
public:
	HTTPServer();
	void start();
	void init(int port = 80);
protected:
	virtual ~HTTPServer();
	static AsyncWebServer *_server;
	String _body;
	int _getBody;
	int getUserRole(AsyncWebServerRequest * request);
	String getClientCookie(AsyncWebServerRequest * request);
	String createCookieString(int role);

	void handleLogin(AsyncWebServerRequest *request);
	void handleLogout(AsyncWebServerRequest *request);
	void handleAjax(AsyncWebServerRequest * request);
	void handleNotFound(AsyncWebServerRequest *request);
	void handleBody(AsyncWebServerRequest  *request, uint8_t *data, size_t len, size_t index, size_t total);
	void sendFile(AsyncWebServerRequest *request,const char * filename, const char *content_type = (const char *) 0, bool isGzip = false, bool needLogin=false, unsigned long expire = 0UL);

};

#endif /* HTTPSERVER_H_ */
