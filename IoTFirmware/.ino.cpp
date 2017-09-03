#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-09-03 15:58:28

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiType.h>
#include <DNSServer.h>
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
void setup() ;
void loop() ;
bool isWifiConnected() ;
void handleWifiDisconnect(const WiFiEventStationModeDisconnected & event) ;
void handleWifiConnect(const WiFiEventStationModeGotIP & event) ;
void connectToWifiAP() ;
void DNSServerStart() ;
void DNSServerStop() ;
void startAPMode() ;
void MQTTInit() ;
void onMqttConnect(bool sessionPresent) ;
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) ;
void onMqttSubscribe(uint16_t packetId, uint8_t qos) ;
void onMqttUnsubscribe(uint16_t packetId) ;
void onMqttMessage(char * topic, char * payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) ;
void onMqttPublish(uint16_t packetId) ;
void connectToMqtt() ;
void handleRoot(AsyncWebServerRequest* request) ;
void putValueOverMQTT() ;
void checkResetButtonStartup() ;

#include "IoTFirmware.ino"


#endif
