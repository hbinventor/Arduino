/*
 * AsyncWebserverEx.cpp
 *
 *  Created on: 12 thg 6, 2017
 *      Author: MyPC
 */

#include "HTTPServer.h"
HTTPServer::HTTPServer():AsyncWebServer(DB::instance()->getHTTPPort())
{
}

void HTTPServer::init()
{
	on("/favicon.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, favicon_png, MimePNG, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/js/jquery-3.1.0.min.js.gz", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, jquery_3_1_0_min_js_gz, MimeTypeJS, GZIP_ENABLE, BROWSER_CACHE_DAYS); });
	on("/js/md5.min.js.gz", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, md5_min_js_gz, MimeTypeJS, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/js/main.js", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, main_js, MimeTypeJS, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/css/main.css", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, main_css, MimeCss, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/admin.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, admin_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/device.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, device_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/email.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, email_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/gpio.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, gpio_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/login.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, login_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/network.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, network_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/settings.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, settings_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/time.html", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, time_html, MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/co22.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, co22_png, MimePNG, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/fire1.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, fire1_png, MimePNG, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/hum2.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, hum22_png, MimePNG, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	on("/temp1.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {HTTPServer::sendFile(request, temp1_png, MimePNG, GZIP_DISABLE, BROWSER_CACHE_DAYS); });

	////on("/js/ionic.bundle.min.js", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/js/ionic.bundle.min.js.gz", MimeTypeJS, GZIP_ENABLE, BROWSER_CACHE_DAYS); });
	////on("/css/ionic.min.css", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/css/ionic.min.css.gz", MimeCss, GZIP_ENABLE, BROWSER_CACHE_DAYS); });
	////on("/css/main.js", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/js/main.js", MimeTypeJS, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	////on("/fonts/ionicons.woff", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/fonts/ionicons.woff", 0, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	////on("/index.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/index.htm", MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	//rewrite("/index.htm", "index-ap.htm").setFilter(ON_AP_FILTER);

	////on(PATH_LOGIN, std::bind(&AsyncWebserverEx::handleLogin, this, std::placeholders::_1));
	////on(PATH_LOGOUT, std::bind(&AsyncWebserverEx::handleLogout, this, std::placeholders::_1));
	//on(PATH_AJAX, std::bind(&AsyncWebserverEx::handleAjax, this, std::placeholders::_1));

	onNotFound(std::bind(&HTTPServer::handleNotFound, this, std::placeholders::_1));
	onRequestBody([&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
		handleBody(request, data, len, index, total);
	});
	//onRequestBody(std::bind(&AsyncWebserverEx::handleBody, this, std::placeholders::_1));
	//onRequestBody(std::bind(&AsyncWebserverEx::handleBody, this, std::placeholders::_1));
	//onFileUpload(std::bind(&AsyncWebserverEx::handleUpload, this, std::placeholders::_1));
}

HTTPServer::~HTTPServer()
{
}



void HTTPServer::start()
{
	begin();
}

/*
 * Lay role tu cookie:
 * Role nay duoc quan ly trong class Session
 * return: 1: admin, 2: user, -1: khong login
 * */
int HTTPServer::getClientRole(AsyncWebServerRequest * request)
{
	return Session::getRole(getClientCookie(request));
}

/*
 * Lay cookie tu client:
 * Cookie dang: hbid=12345678901234567890123456789012
 * return: 12345678901234567890123456789012
 * */
String HTTPServer::getClientCookie(AsyncWebServerRequest * request)
{
	if (request->hasHeader("Cookie")) {
		// Lay header cookie
		AsyncWebHeader* h = request->getHeader("Cookie");
		String cookie = h->value();
		DBG2F0("Client Cookie: %s\n", cookie);

		// Xoa bo phan prefix: hbid=
		// lay 32 ky tu phia sau - 5 -> 37
		int start = cookie.indexOf("hbid=");
		return cookie.substring(start + 5, start + 37);
	}
	// Neu khong tim thay cookie tra ve chuoi rong
	return "";
}

/*
 * Tao chuoi Cookie de add vao header gui toi client
 * Chuoi nay duoc quan ly trong class session
 * return: chuoi header: hbid=12345678901234567890123456789012; Path=/; Max-Age= 86400L*/
String HTTPServer::newCookieString(int role)
{
	String cookie;
	String ssName = Session::create(role);
	// Khoi tao prefix "hbid="
	cookie = "hbid=";
	// Tao chuoi cookie
	cookie += ssName;
	// Them Path va thoi gian hieu luc cho cookie nay
	cookie += "; Path=/; Max-Age=";
	cookie += SESSION_EXPIRE_TIME;
	DBG2F("Set-Cookie: ", cookie);
	return cookie;
}

/*
 * Xu ly login tu client
 * */
void HTTPServer::handleLogin(AsyncWebServerRequest * request)
{
	int role = getClientRole(request);
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
				response->addHeader(HEADER_SET_COOKIE, newCookieString(USER_ROLE));
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
				response->addHeader(HEADER_SET_COOKIE, newCookieString(ADMIN_ROLE));
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

/*
 * Xu ly client logout*/
void HTTPServer::handleLogout(AsyncWebServerRequest * request)
{
	// Lay cookie
	String ss = getClientCookie(request);
	DBG2F("Logout => Delete Session: ", ss);
	// Xoa session cua client
	Session::remove(ss);
	// Dieu huong den trang Login
	request->redirect(PATH_LOGIN);
}

void HTTPServer::handleAjax(AsyncWebServerRequest * request)
{
	AsyncResponseStream *response = request->beginResponseStream("text/json");
	DynamicJsonBuffer jsonBuffer;
	JsonObject &root = jsonBuffer.createObject();
	int role = getClientRole(request);
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
		int role = getClientRole(request);
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
	if (needLogin && (getClientRole(request)<=0))
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

