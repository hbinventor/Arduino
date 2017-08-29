
#include <ESP8266WiFi.h>
#include <ESPAsyncUDP.h>
#include <ESP8266WiFiType.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <Timezone.h>
#include "TimeService.h"
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
#include <FS.h>
#include <TimeLib.h>
#include <Ticker.h>
#include <tcp_axtls.h>
#include <SyncClient.h>
#include <ESPAsyncTCPbuffer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <async_config.h>
#include <AsyncPrinter.h>
#include <AsyncWebSocket.h>
#include <AsyncJson.h>
#include <AsyncEventSource.h>
#include <AsyncMqttClient.h>
#include <DNSServer.h>
#include <WebResponseImpl.h>
#include <WebHandlerImpl.h>
#include <WebAuthentication.h>
#include <StringArray.h>
#include <SPIFFSEditor.h>
#include <WiFiUdp.h>
#include "Configs.h"
#include "Singleton.h"
#include "DB.h"
#include "GPIOs.h"
#include "HTTPServer.h"
#include "OTAUpdater.h"
#include "BMQTT.h"
#include "BWifi.h"
#include "BWebAPI.h"
#include "EventHandle.h"

#define gDB	DB::instance()
#define gWF	WifiManager::instance()
#define	gHTTP	HTTPServer::getInstance()
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
	// Thiet lap port cho HTTP server
	gHTTP->init(gDB->getHTTPPort());

	// Cac handle phat hien wifi event
	BWifi::instance()->onStationModeGotIP(handleWifiConnect);
	BWifi::instance()->onStationModeDisconnected(handleWifiDisconnect);

	// Thiet lap TimeService: Time server va Timezone
	gTime->setNTPServer(gDB->getTimeServer().c_str());
	gTime->setTimeZone(gDB->getTimeZone());

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
	return BWifi::instance()->isConnected();
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
	DBG(BWifi::instance()->localIP());

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
	BWifi::instance()->setAutoConnect(true);
	if (gDB->getWifiPassword().length() != 0)	// Neu mang Wifi dung mat khau
	{
		BWifi::instance()->begin(gDB->getSSID().c_str(), gDB->getWifiPassword().c_str());
	}
	else// Wifi Open
	{
		BWifi::instance()->begin(gDB->getSSID().c_str());
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
	BWifi::instance()->mode(WIFI_AP_STA);
	// Thiet lap IP cho AP
	IPAddress ip;
	IPAddress gw;
	IPAddress msk;
	ip.fromString(gDB->getIP(true));
	gw.fromString(gDB->getGateway(true));
	msk.fromString(gDB->getMask(true));
	BWifi::instance()->softAPConfig(ip, gw, msk);
	// Thiet lap ten SSID va mat khau
	BWifi::instance()->softAP(gDB->getDeviceSerial().c_str(), gDB->getWifiPassword(true).c_str());
	// Bat DNS server dieu huowng ket noi ve AP IP
	DNSServerStart();
}


/*******************************************************************
*	Area of MQTT function
********************************************************************/
void MQTTInit()
{
	// Thiet lap cac handle MQTT
	BMQTT::getInstance()->onConnect(onMqttConnect);
	BMQTT::getInstance()->onConnect(onMqttConnect);
	BMQTT::getInstance()->onDisconnect(onMqttDisconnect);
	BMQTT::getInstance()->onSubscribe(onMqttSubscribe);
	BMQTT::getInstance()->onUnsubscribe(onMqttUnsubscribe);
	BMQTT::getInstance()->onMessage(onMqttMessage);
	BMQTT::getInstance()->onPublish(onMqttPublish);
	// Thiet lap cho MQTT Client
	BMQTT::getInstance()->setServer(gDB->getMQTTBroker().c_str(), gDB->getMQTTPort());
	BMQTT::getInstance()->setCredentials(gDB->getMQTTUsername().c_str(), gDB->getMQTTPassword().c_str());
}
// Handle on MQTT connect to broker
void onMqttConnect(bool sessionPresent)
{
	DBGF("Connected to MQTT.");
	DBG2F("Session present: ", sessionPresent);
	BMQTT::getInstance()->listen();
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
	if (BWifi::instance()->isConnected()) {
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
	BMQTT::getInstance()->connect();
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
