#include <ESP8266WiFi.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiType.h>
#include <DNSServer.h>
// User libraries
#include "Configs.h"
#include "Singleton.h"
#include "DB.h"
#include "GPIOs.h"
#include "OTAUpdater.h"
#include "BMQTT.h"
#include "BWifi.h"
#include "BWebAPI.h"
#include "BWebServer.h"
#include "TimeService.h"
#include "EventHandle.h"

// HTML static file
#include "html/favicon.png.h"
#include "html/co22.png.h"
#include "html/fire1.png.h"
#include "html/hum2.png.h"
#include "html/temp1.png.h"
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

#define gDB	DB::instance()
#define gWF	WifiManager::instance()
#define	gHTTP	BWebServer::instance()
#define	gTime	TimeService::instance()

/************************************************************************
*						Declaration VARIABLES							*
*************************************************************************/
time_t a;
// System define
typedef enum {Setup, Normal} BootMode_t;
BootMode_t BootMode;
void checkResetButtonStartup();

// Wifi function
Ticker WifiConnTicker;
void handleWifiDisconnect(const WiFiEventStationModeDisconnected& event);
void handleWifiConnect(const WiFiEventStationModeGotIP& event);
void startAPMode();
void connectToWifiAP();
void checkFisrtWifiConn();
bool isWifiConnected();

// DNS Server
DNSServer *DnsServer;
void DNSServerStart();
void DNSServerStop();

// Webserver
void handleRoot(AsyncWebServerRequest *request);
// MQTT
Ticker MQTTTicker;
void MQTTInit();
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void onMqttPublish(uint16_t packetId);
void connectToMqtt();
Ticker MQTTTestTicker;
void putValueOverMQTT();
/********************************************************************************
*									Run Area									*
*********************************************************************************/
void setup() {
	INIT_SERIAL_DEBUG();
	delay(100);
	DBG("Hello From System");
	gDB->load();
	checkResetButtonStartup();
	if (BootMode==Setup)
	{
		startAPMode();
		return;
	}

	// Cac handle phat hien wifi event
	WiFi.onStationModeGotIP(handleWifiConnect);
	WiFi.onStationModeDisconnected(handleWifiDisconnect);

	// Thiet lap port cho HTTP server
	gHTTP->begin();

	MQTTInit();

	MQTTTestTicker.attach(1, putValueOverMQTT);
	GPIOs::instance()->checkUpdate();
	gHTTP->start();

	connectToWifiAP();
}
// the loop function runs over and over again until power down or reset
void loop() {
	yield();
	GPIOs::instance()->loop();
	if (DnsServer != nullptr) DnsServer->processNextRequest();
}

/*******************************************************************
*	Area of Wifi function
********************************************************************/
bool isWifiConnected()
{
	return WiFi.isConnected();
}
// Handle khi bi mat ket noi wifi
void handleWifiDisconnect(const WiFiEventStationModeDisconnected & event)
{
	// Ngat ticker tu dong ket noi MQTT
	MQTTTicker.detach();
	// Bat ticker tu dong ket noi lai Wifi sau 5s
	WifiConnTicker.once(5, connectToWifiAP);
}
// Handle khi ket noi wifi thanh cong
void handleWifiConnect(const WiFiEventStationModeGotIP & event)
{
	DBGF("Wifi Station got IP: ");
	DBG(WiFi.localIP());

}

void connectToWifiAP()
{
	BWifi::instance()->scanToList();
	// Kiem tra xem co dung DHCP
	if (!gDB->isDHCP()) {	// Mode static IP
		DBG("Using static IP");
		IPAddress ip;
		IPAddress gw;
		IPAddress msk;
		ip.fromString(gDB->getIP());	// Lay thong tin static IP tu EEPROM
		gw.fromString(gDB->getGateway());
		msk.fromString(gDB->getMask());
		WiFi.config(ip, gw, msk, IPAddress(8, 8, 8, 8), IPAddress(208, 67, 222, 222));	// Luu thiet lap vao BWifi Class
	}
	else {
		DBG("Using DHCP");
	}

	DBG("Connecting to WIFI.");
	DBG2F("SSID:", gDB->getSSID());
	DBG2F("PW:", gDB->getWifiPassword());
	// Set che do auto connect, chua biet co hoat dong khong, SDK noi vay
	WiFi.setAutoConnect(true);
	if (gDB->getWifiPassword().length() != 0)	// Neu mang Wifi dung mat khau
	{
		WiFi.begin(gDB->getSSID().c_str(), gDB->getWifiPassword().c_str());
	}
	else// Wifi Open
	{
		WiFi.begin(gDB->getSSID().c_str());
	}
}

void DNSServerStart()
{
	if (DnsServer != nullptr)
	{
		return;
	}
	else
	{
		DnsServer = new DNSServer();
		IPAddress ip;
		// Khoi tao DNS server, dieu huong tat ca ket noi ve IP AP
		DnsServer->setErrorReplyCode(DNSReplyCode::NoError);
		DnsServer->start(53, "*", ip.fromString(gDB->getIP(true)));
	}
}

void DNSServerStop()
{
	if (DnsServer != nullptr)
	{
		DnsServer = nullptr;
	}
}

void startAPMode() {
	// set mode AP_STA de scan network khi cai dat
	WiFi.mode(WIFI_AP_STA);
	// Thiet lap IP cho AP
	IPAddress ip;
	IPAddress gw;
	IPAddress msk;
	ip.fromString(gDB->getIP(true));
	gw.fromString(gDB->getGateway(true));
	msk.fromString(gDB->getMask(true));
	//WiFi.softAPConfig(ip, gw, msk);
	// Thiet lap ten SSID va mat khau
	//WiFi.softAP(gDB->getDeviceSerial().c_str(), gDB->getWifiPassword(true).c_str());
	// Bat DNS server dieu huowng ket noi ve AP IP
	DNSServerStart();
}


/*******************************************************************
*	Area of MQTT function
********************************************************************/
void MQTTInit()
{
	// Thiet lap cac handle MQTT
	BMQTT::instance()->onConnect(onMqttConnect);
	BMQTT::instance()->onConnect(onMqttConnect);
	BMQTT::instance()->onDisconnect(onMqttDisconnect);
	BMQTT::instance()->onSubscribe(onMqttSubscribe);
	BMQTT::instance()->onUnsubscribe(onMqttUnsubscribe);
	BMQTT::instance()->onMessage(onMqttMessage);
	BMQTT::instance()->onPublish(onMqttPublish);
	// Thiet lap cho MQTT Client
	BMQTT::instance()->setServer(gDB->getMQTTBroker().c_str(), gDB->getMQTTPort());
	BMQTT::instance()->setCredentials(gDB->getMQTTUsername().c_str(), gDB->getMQTTPassword().c_str());
}
// Handle on MQTT connect to broker
void onMqttConnect(bool sessionPresent)
{
	DBGF("Connected to MQTT.");
	DBG2F("Session present: ", sessionPresent);
	BMQTT::instance()->listen();
	/*
	uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
	Serial.print("Subscribing at QoS 2, packetId: ");
	Serial.println(packetIdSub);
	mqttClient.publish("test/lol", 0, true, "test 1");
	Serial.println("Publishing at QoS 0");
	uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
	Serial.print("Publishing at QoS 1, packetId: ");
	Serial.println(packetIdPub1);
	uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
	Serial.print("Publishing at QoS 2, packetId: ");
	Serial.println(packetIdPub2);
	*/
}
// Handle when MQTT disconnect from broker
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
	DBGF("Disconnected from MQTT.");
	if (WiFi.isConnected()) {
		DBGF("Connecting to MQTT...");
		MQTTTicker.once(2, connectToMqtt);
	}
}
// Handle when Subscribe success
void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
	DBGF("Subscribe acknowledged.\t");
	DBGF0("packetId:\t");
	DBG(packetId);
	DBGF0("qos:\t");
	DBG(qos);
}
// Handle when Unsubscribe succes
void onMqttUnsubscribe(uint16_t packetId)
{
	DBGF("Unsubscribe acknowledged.");
	DBGF0("packetId: \t");
	DBG(packetId);
}
// Handle when receive message from MQTT
void onMqttMessage(char * topic, char * payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
	Serial.println("Publish received.");
	Serial.print("  topic: ");
	Serial.println(topic);
	Serial.print("  qos: ");
	Serial.println(properties.qos);
	Serial.print("  dup: ");
	Serial.println(properties.dup);
	Serial.print("  retain: ");
	Serial.println(properties.retain);
	Serial.print("  len: ");
	Serial.println(len);
	Serial.print("  index: ");
	Serial.println(index);
	Serial.print("  total: ");
	Serial.println(total);
	//EventHandle::instance()->onMQTTMessage(topic, payload);
}
// Handle when Publish succes
void onMqttPublish(uint16_t packetId)
{
	DBGF("Publish acknowledged.");
	DBGF0("packetId: \t");
	DBG(packetId);
}
// Connect to MQTT
void connectToMqtt()
{
	DBGF("Connecting to MQTT...");
	BMQTT::instance()->connect();
}

void handleRoot(AsyncWebServerRequest* request) {
}

// Put value to Device Topic
void putValueOverMQTT()
{
	String meTopic = TOPIC_PRIVATE_PREFIX;
	meTopic += "/";
	meTopic += DB::instance()->getDeviceSerial();
	meTopic += "/control";
	long a = random(1,100);
	if (a%2)
	{
		//BMQTT::getInstance()->publish(meTopic.c_str(), "{\"pin\":0,\"value\":1}");
	}
	else
	{
		//BMQTT::getInstance()->publish(meTopic.c_str(), "{\"pin\":0,\"value\":0}");
	}

}

/***************************************************************************
*	Hard button function
****************************************************************************/

void checkResetButtonStartup() {
	// Kiem tra nut setup Mode
	pinMode(RESET_PIN, INPUT_PULLUP);
	if(!digitalRead(RESET_PIN)){
		delay(3000);
		if(digitalRead(RESET_PIN)){
			// ham goi khi nhan giu 3s, vao setup mode
			BootMode = Setup;
			return;
		}
		delay(3000);
		if(digitalRead(RESET_PIN)){
			// ham goi khi nhan giu 6s- reset password

			return;
		}
		delay(3000);
		if(digitalRead(RESET_PIN)){
			// ham goi khi nhan giu 9s - restore mac dinh nha sx

			return;
		}
	}
	BootMode = Normal;
}
