/*
 * AsyncWebserverEx.cpp
 *
 *  Created on: 12 thg 6, 2017
 *      Author: MyPC
 */

#include "HTTPServer.h"
AsyncWebServer *HTTPServer::_server;
HTTPServer::HTTPServer():AsyncWebServer(DB::instance()->getHTTPPort())
{
}

void HTTPServer::init(int port)
{

	_server = new AsyncWebServer(port);

	_server->on("/favicon.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, favicon_png, MimePNG, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/js/jquery-3.1.0.min.js.gz", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, jquery_3_1_0_min_js_gz, MimeTypeJS, GZIP_ENABLE, BROWSER_CACHE_DAYS); });
	_server->on("/js/md5.min.js.gz", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, md5_min_js_gz, MimeTypeJS, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/js/main.js", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, main_js, MimeTypeJS, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/css/main.css", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, main_css, MimeCss, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/admin.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, admin_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/device.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, device_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/email.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, email_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/gpio.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, gpio_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/login.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, login_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/network.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, network_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/settings.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, settings_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/time.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, time_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/co22.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, co22_png, MimePNG, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/fire1.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, fire1_png, MimePNG, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/hum2.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, hum22_png, MimePNG, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	_server->on("/temp1.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, temp1_png, MimePNG, GZIP_DISABLE, BROWSER_CACHE_DAYS); });

	////_server->on("/js/ionic.bundle.min.js", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/js/ionic.bundle.min.js.gz", MimeTypeJS, GZIP_ENABLE, BROWSER_CACHE_DAYS); });
	////_server->on("/css/ionic.min.css", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/css/ionic.min.css.gz", MimeCss, GZIP_ENABLE, BROWSER_CACHE_DAYS); });
	////_server->on("/css/main.js", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/js/main.js", MimeTypeJS, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	////_server->on("/fonts/ionicons.woff", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/fonts/ionicons.woff", 0, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	////_server->on("/index.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/index.htm", MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	//_server->rewrite("/index.htm", "index-ap.htm").setFilter(ON_AP_FILTER);

	////_server->on(PATH_LOGIN, std::bind(&AsyncWebserverEx::handleLogin, this, std::placeholders::_1));
	////_server->on(PATH_LOGOUT, std::bind(&AsyncWebserverEx::handleLogout, this, std::placeholders::_1));
	//_server->on(PATH_AJAX, std::bind(&AsyncWebserverEx::handleAjax, this, std::placeholders::_1));

	_server->onNotFound(std::bind(&HTTPServer::handleNotFound, this, std::placeholders::_1));
	_server->onRequestBody([&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		handleBody(request, data, len, index, total);
	});
	//_server->onRequestBody(std::bind(&AsyncWebserverEx::handleBody, this, std::placeholders::_1));
	//_server->onRequestBody(std::bind(&AsyncWebserverEx::handleBody, this, std::placeholders::_1));
	//_server->onFileUpload(std::bind(&AsyncWebserverEx::handleUpload, this, std::placeholders::_1));
	startSession();
}

HTTPServer::~HTTPServer()
{
	 _server = nullptr;
	SS = nullptr;
}



void HTTPServer::start()
{
	_server->begin();
}

int HTTPServer::getUserRole(AsyncWebServerRequest * request)
{
	return getSSUserRole(getClientCookie(request));
}

String HTTPServer::getClientCookie(AsyncWebServerRequest * request)
{
	if (request->hasHeader("Cookie")) {
		AsyncWebHeader* h = request->getHeader("Cookie");
		DBG2F0("Client Cookie: %s\n", h->value());
		String cookie = h->value();
		int start = cookie.indexOf("hbid=");
		return cookie.substring(start + 5, start + 37);
	}
	return "";
}

String HTTPServer::createCookieString(int role)
{
	String cookie;
	String ssName = createSS(role);
	cookie = "hbid=";
	cookie += ssName;
	cookie += "; Path=/; Max-Age=";
	cookie += SESSION_EXPIRE_TIME;
	DBG2F("Set-Cookie: ", cookie);
	return cookie;
}

void HTTPServer::handleLogin(AsyncWebServerRequest * request)
{
	int role = getUserRole(request);
	if (role>0)
	{
		request->redirect(PATH_ROOT);
		return;
	}
	if (!request->hasParam(PAR_USERNAME)|| !request->hasParam(PAR_PASSWORD))
	{
		request->redirect(PATH_LOGIN);
		return;
	}
	else
	{
		AsyncWebParameter* p = request->getParam(PAR_USERNAME);
		String username = p->value();
		p = request->getParam(PAR_PASSWORD);
		String password = p->value();
		AsyncResponseStream *response = request->beginResponseStream("text/json");
		DynamicJsonBuffer jsonBuffer;
		JsonObject &root = jsonBuffer.createObject();
		if (username == "user") {
			if (password == DB::instance()->getLoginPassword())
			{
				response->addHeader(HEADER_SET_COOKIE, createCookieString(USER_ROLE));
				root[PAR_STATUS] = STT_OK;
				root.printTo(*response);
				request->send(response);
			}
			else
			{
				response->addHeader(HEADER_CONNECTION, "close");
				root[PAR_STATUS] = STT_WRONG_PASS;
				root.printTo(*response);
				request->send(response);
			}
		}
		else if (username == "admin") {
			if (password == DB::instance()->getLoginPassword(true))
			{
				response->addHeader(HEADER_SET_COOKIE, createCookieString(ADMIN_ROLE));
				root[PAR_STATUS] = STT_OK;
				root.printTo(*response);
				request->send(response);
			}
			else
			{
				response->addHeader(HEADER_CONNECTION, "close");
				root[PAR_STATUS] = STT_WRONG_PASS;
				root.printTo(*response);
				request->send(response);
			}
		}
		else {
			response->addHeader(HEADER_CONNECTION, "close");
			root[PAR_STATUS] = STT_WRONG_USER;
			root.printTo(*response);
			request->send(response);
		}
	}
}

void HTTPServer::handleLogout(AsyncWebServerRequest * request)
{
	String ss = getClientCookie(request);
	DBG2F("Logout => Delete Session: ", ss);
	deleteSS(ss);
	request->redirect(PATH_LOGIN);
}

void HTTPServer::handleAjax(AsyncWebServerRequest * request)
{
	AsyncResponseStream *response = request->beginResponseStream("text/json");
	DynamicJsonBuffer jsonBuffer;
	JsonObject &root = jsonBuffer.createObject();
	int role = getUserRole(request);
	if (role<=0 || !request->hasParam(PAR_USERNAME))
	{
		root[PAR_STATUS] = "false";
		root.printTo(*response);
		request->send(response);
		return;
	}
	else
	{
		AsyncWebParameter* command = request->getParam(PAR_COMMAND);
		String username = command->value();

	}
}

void HTTPServer::handleNotFound(AsyncWebServerRequest * request)
{
	//Handle Unknown Request
	request->send(404);
}

void HTTPServer::handleBody(AsyncWebServerRequest * request, uint8_t * data, size_t len, size_t index, size_t total)
{
	if (!index) {
		_body = "";
		_getBody = 0;
	}
	for (size_t i = 0; i<len; i++) {
		_body += data[i];
	}
	if (index + len == total) {
		_getBody = 1;
		DBG2F("Body : ", _body);
		int role = getUserRole(request);
		if (role > 0)
		{
			String res = Commander::instance()->processWebsever(_body.c_str());
			AsyncResponseStream *response = request->beginResponseStream("text/json");
			response->addHeader(HEADER_CONNECTION, "close");
			response->print(res);
			request->send(response);
			return;
		}
	}
}

void HTTPServer::sendFile(AsyncWebServerRequest *request, const char * filename, const char *content_type, bool isGzip, bool needLogin, unsigned long expire)
{
	if (needLogin && (getUserRole(request)<=0))
	{
		DBGF("This file need login");
		request->redirect(PATH_LOGIN);
		return;
	}
	AsyncWebServerResponse *response = request->beginResponse_P(200, content_type, filename);
	if (isGzip)
	{
		response->addHeader("Content-Encoding", "gzip");
	}
	if (expire)
	{
		char buf[34];
		sprintf(buf, "max-age=%lu", expire);
		response->addHeader("Cache-Control", buf);
	}
	if (content_type)
	{
		response->setContentType(content_type);
	}
	response->addHeader("Connection", "close");
	request->send(response);
}

