/*
 * AsyncWebserverEx.cpp
 *
 *  Created on: 12 thg 6, 2017
 *      Author: MyPC
 */

#include "BWebServer.h"
BWebServer::BWebServer():AsyncWebServer(DB::instance()->getHTTPPort())
{
	// Static file
	on("/favicon.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, favicon_png, MimePNG, false); });
	on("/co22.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, co22_png, MimePNG, false); });
	on("/fire1.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, fire1_png, MimePNG, false); });
	on("/hum2.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, hum22_png, MimePNG, false); });
	on("/temp1.png", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, temp1_png, MimePNG, false); });
	on("/js/jquery-3.1.0.min.js.gz", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, jquery_3_1_0_min_js_gz, MimeTypeJS); });
	on("/js/md5.min.js.gz", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, md5_min_js_gz, MimeTypeJS); });
	on("/js/main.js", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, main_js, MimeTypeJS, false); });
	on("/css/main.css", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, main_css, MimeCss, false); });
	on("/admin.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, admin_html, MimeHtml, false); });
	on("/device.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, device_html, MimeHtml, false); });
	on("/email.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, email_html, MimeHtml, false); });
	on("/gpio.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, gpio_html, MimeHtml, false); });
	on("/login.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, login_html, MimeHtml, false); });
	on("/network.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, network_html, MimeHtml, false); });
	on("/settings.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, settings_html, MimeHtml, false); });
	on("/time.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, time_html, MimeHtml, false); });
	on("/index.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {BWebServer::sendStaticFile(request, admin_html, MimeHtml, false); });

	onNotFound(std::bind(&BWebServer::handleNotFound, this, std::placeholders::_1));
	// Ajax

}

void BWebServer::init()
{
	////on("/js/ionic.bundle.min.js", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/js/ionic.bundle.min.js.gz", MimeTypeJS, GZIP_ENABLE, BROWSER_CACHE_DAYS); });
	////on("/css/ionic.min.css", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/css/ionic.min.css.gz", MimeCss, GZIP_ENABLE, BROWSER_CACHE_DAYS); });
	////on("/css/main.js", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/js/main.js", MimeTypeJS, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	////on("/fonts/ionicons.woff", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/fonts/ionicons.woff", 0, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	////on("/index.htm", HTTP_ANY, [&](AsyncWebServerRequest *request) {AsyncWebserverEx::sendFile(request, "/index.htm", MimeHtml, GZIP_DISABLE, BROWSER_CACHE_DAYS); });
	//rewrite("/index.htm", "index-ap.htm").setFilter(ON_AP_FILTER);

	////on(PATH_LOGIN, std::bind(&AsyncWebserverEx::handleLogin, this, std::placeholders::_1));
	////on(PATH_LOGOUT, std::bind(&AsyncWebserverEx::handleLogout, this, std::placeholders::_1));
	//on(PATH_AJAX, std::bind(&AsyncWebserverEx::handleAjax, this, std::placeholders::_1));
	//onRequestBody(std::bind(&AsyncWebserverEx::handleBody, this, std::placeholders::_1));
	//onRequestBody(std::bind(&AsyncWebserverEx::handleBody, this, std::placeholders::_1));
	//onFileUpload(std::bind(&AsyncWebserverEx::handleUpload, this, std::placeholders::_1));
}

BWebServer::~BWebServer()
{
}



void BWebServer::start()
{
	begin();
}

/*
 * Lay role tu cookie:
 * Role nay duoc quan ly trong class Session
 * return:NONE, USER, ADMIN
 * */
USER_ROLE BWebServer::getClientRole(AsyncWebServerRequest * request)
{
	return Session::getRole(getClientCookie(request));
}

/*
 * Lay cookie tu client:
 * Cookie dang: hbid=12345678901234567890123456789012
 * return: 12345678901234567890123456789012
 * */
String BWebServer::getClientCookie(AsyncWebServerRequest * request)
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
 * Send static file to client ex: html, js, css, image...etc
 * */
void BWebServer::sendStaticFile(AsyncWebServerRequest* request,
		const char* filename, const char* content_type, bool isGzip, USER_ROLE role) {
	sendFile(request, filename, content_type, isGzip, role, BROWSER_CACHE_DAYS);
}

/*
 * Tao chuoi Cookie de add vao header gui toi client
 * Chuoi nay duoc quan ly trong class session
 * return: chuoi header: hbid=12345678901234567890123456789012; Path=/; Max-Age= 86400L*/
String BWebServer::newCookieString(USER_ROLE role)
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
 * Xu ly cac trang khong tim thay
 * Error code 404
 * */
void BWebServer::handleNotFound(AsyncWebServerRequest * request)
{
	/*	Handle Unknown Request	*/
	request->send(404);
}

/*
 * Xu ly login tu client
 * */
void BWebServer::handleLogin(AsyncWebServerRequest * request)
{
	/* Lay client role tu cookie	*/
	USER_ROLE clientRole = getClientRole(request);
	/* neu da dang nhap (ROLE != NONE --- ADMIN or USER) thi chuyen huong den trang chu	*/
	if (clientRole > NONE)
	{
		request->redirect(PATH_ROOT);
		return;
	}
	/* neu request gui len chua du thong tin user va password thi load lai trang login	*/
	if (!request->hasParam(PAR_USERNAME)|| !request->hasParam(PAR_PASSWORD))
	{
		request->redirect(PATH_LOGIN);
		return;
	}

	/*	 Neu da day du thong tin request: usernam, password, chua dang nhap thi xu ly login */
	else
	{
		/* lay request param username va password	*/
		AsyncWebParameter* p = request->getParam(PAR_USERNAME);
		String username = p->value();
		p = request->getParam(PAR_PASSWORD);
		String password = p->value();

		/* Khoi tao chuoi json tra ve */
		AsyncResponseStream *response = request->beginResponseStream("text/json");
		DynamicJsonBuffer jsonBuffer;
		JsonObject &root = jsonBuffer.createObject();

		/* Check username: user login*/
		if (username == "user") {
			/*	Neu dung username: user va password*/
			if (password == DB::instance()->getLoginPassword())
			{
				/* Tra lai client chuoi json: {"status": true}	*/
				response->addHeader("Set-Cookie", newCookieString(USER));
				root["status"] = true;
				root.printTo(*response);
				request->send(response);
			}
			/*	Neu sai thong tin dang nhap*/
			else
			{
				/* Tra lai client chuoi json: {"status": false}	*/
				response->addHeader(HEADER_CONNECTION, "close");
				root["status"] = false;
				root.printTo(*response);
				request->send(response);
			}
		}
		/* Check username: admin login*/
		else if (username == "admin") {
			/*	Neu dung username: admin va password*/
			if (password == DB::instance()->getLoginPassword(true))
			{
				/* Tra lai client chuoi json: {"status": true}	*/
				response->addHeader("Set-Cookie", newCookieString(ADMIN));
				root[PAR_STATUS] = STT_OK;
				root.printTo(*response);
				request->send(response);
			}
			/*	Neu sai thong tin dang nhap*/
			else
			{
				/* Tra lai client chuoi json: {"status": false}	*/
				response->addHeader(HEADER_CONNECTION, "close");
				root[PAR_STATUS] = STT_WRONG_PASS;
				root.printTo(*response);
				request->send(response);
			}
		}
		/* Neu username khac user hoac admin, tra lai client chuoi json: {"status": false}*/
		else {
			response->addHeader(HEADER_CONNECTION, "close");
			root["status"] = false;
			root.printTo(*response);
			request->send(response);
		}
	}
}

/*
 * Xu ly client logout*/
void BWebServer::handleLogout(AsyncWebServerRequest * request)
{
	/*	Lay session trong client cookie */
	String ss = getClientCookie(request);
	DBG2F("Logout => Delete Session: ", ss);

	/* Xoa session cua client	*/
	Session::remove(ss);

	/* Dieu huong den trang Login	*/
	request->redirect(PATH_LOGIN);
}

void BWebServer::handleAjax(AsyncWebServerRequest * request)
{
	AsyncResponseStream *response = request->beginResponseStream("text/json");
	DynamicJsonBuffer jsonBuffer;
	JsonObject &root = jsonBuffer.createObject();
	USER_ROLE role = getClientRole(request);
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

void BWebServer::handleBody(AsyncWebServerRequest * request, uint8_t * data, size_t len, size_t index, size_t total)
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

void BWebServer::sendFile(AsyncWebServerRequest *request, const char * filename, const char *content_type, bool isGzip, USER_ROLE role, unsigned long expire)
{
	if (getClientRole(request)<role)
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

void BWebServer::sendGzipFile(AsyncWebServerRequest* request,
		const char* filename, const char* content_type, USER_ROLE role,
		unsigned long expire) {
	sendFile(request, filename, content_type, true, role, expire);
}
