#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2017-08-29 15:20:56

#include "Arduino.h"
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
void putValueOverMQTT() ;
void checkResetButtonStartup() ;

#include "IoTFirmware.ino"


#endif
