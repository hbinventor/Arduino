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
 * return:NONE, USER, ADMIN
 * */
USER_ROLE HTTPServer::getClientRole(AsyncWebServerRequest * request)
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
String HTTPServer::newCookieString(USER_ROLE role)
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
void HTTPServer::handleNotFound(AsyncWebServerRequest * request)
{
	/*	Handle Unknown Request	*/
	request->send(404);
}

/*
 * Xu ly login tu client
 * */
void HTTPServer::handleLogin(AsyncWebServerRequest * request)
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
void HTTPServer::handleLogout(AsyncWebServerRequest * request)
{
	/*	Lay session trong client cookie */
	String ss = getClientCookie(request);
	DBG2F("Logout => Delete Session: ", ss);

	/* Xoa session cua client	*/
	Session::remove(ss);

	/* Dieu huong den trang Login	*/
	request->redirect(PATH_LOGIN);
}

void HTTPServer::handleAjax(AsyncWebServerRequest * request)
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

void HTTPServer::sendGzipFile(AsyncWebServerRequest* request,
		const char* filename, const char* content_type, bool needLogin,
		unsigned long expire) {
	sendFile(request, filename, content_type, true, needLogin, expire);
}
