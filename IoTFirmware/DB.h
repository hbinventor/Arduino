#ifndef _MODULE_SETTINGS_H_
#define _MODULE_SETTINGS_H_

#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "Singleton.h"
#include "Configs.h"
/*******************************************************************
************** 	Kich thuoc cac bien trong EEPROM		********
*******************************************************************/
#define SETTING_BLOCK_START   0
#define ADMIN_ROLE   1
#define USER_ROLE    2
#define CHECKSUM_LEN 32
#define DEVICE_NAME_LEN 32
#define DEVICE_TYPE_LEN 16
#define DEVICE_SERIAL_LEN 16
#define LOGIN_PASSWORD_LEN 16
#define ENCRYPT_KEY_LEN 16
#define SSID_LEN 33
#define WIFI_PASSWORD_LEN 32
#define IP_LEN 15
#define TIME_SERVER_LEN 32
#define MQTT_BROKER_LEN	48
#define MQTT_USERNAME_LEN	32
#define MQTT_PASSWORD_LEN	32

#define AP_SETTING	true
#define KEY_PRIVATE	true
#define ADMIN_SETTING	true
// change this value to set the SMS Count value when updating ApiKey
/*******************************************************************
************** 	Cac gia tri mac dinh		********************
* *****************************************************************/
#define DEFAULT_SMS_COUNT   10000
/////////////////////////////////////////////////////////////////////////////////////////

#define DEF_TIME_SERVER	"pool.ntp.org"
#define DEF_ADMIN_PASSWORD	"Sesame1+"
#define DEF_USER_PASSWORD	"12345678"
#define DEF_ENCRYTKEY_PUBLIC	""
#define DEF_ENCRYTKEY_PRIVATE	""
#define DEF_SSID_STA	"HBInvent"
#define DEF_PASSWORD_STA	"!()(!(*("
#define DEF_PASSWORD_AP	""
#define DEF_IP_STA	""
#define DEF_IP_AP	"192.168.3.1"
#define DEF_MASK_STA	""
#define DEF_MASK_AP	"255.255.255.0"
#define DEF_GW_STA	""
#define DEF_GW_AP	"192.168.3.1"
#define DEF_MQTT_BROKER	"mqtt.hbinvent.com"
#define DEF_MQTT_USER	"19D5E9D6E86834E05CB27D7D0703E30C"
#define DEF_MQTT_PASSWORD	"19D5E9D6E86834E05CB27D7D0703E30C"

#define DEF_DATA_ENDING	8

#define OUTPUT_PINS	5
#define OUTPUT_PIN_1	12	// outputs pin
#define OUTPUT_FB_1	12	// outputs feedback pin
#define OUTPUT_PIN_2	13	// outputs pin
#define OUTPUT_FB_2	13	// outputs feedback pin
#define OUTPUT_PIN_3	14	// outputs pin
#define OUTPUT_FB_3	14	// outputs feedback pin
#define OUTPUT_PIN_4	15	// outputs pin
#define OUTPUT_FB_4	15	// outputs feedback pin
#define OUTPUT_PIN_5	16	// outputs pin
#define OUTPUT_FB_5	16	// outputs feedback pin
#define DEF_TIMEON_TOGGLE	0
#define DEF_TIMEOFF_TOGGLE	0
#define DEF_TIMEON_BLINK	500	// ms
#define DEF_TIMEOFF_BLINK	500	// ms

#define INPUT_PINS	5

// MQTT struct

#define TOPIC_SUB_MAX	10
#define TOPIC_SUB_LEN	64
typedef struct
__attribute__((packed)) {
	bool enable;
	char topic[TOPIC_SUB_LEN + 1];
} MQTTTopic_t;

typedef struct __attribute__((packed)) Setting_t {
	char MD5[CHECKSUM_LEN + 1];	// checksum
	char Name[DEVICE_NAME_LEN + 1];	// Device Name
	char Type[DEVICE_TYPE_LEN + 1];	// Device Type
	char Serial[DEVICE_SERIAL_LEN + 1];	// AP WIFI WifiSsid

	char AdminPassword[LOGIN_PASSWORD_LEN + 1];	// Login password cho admin
	char UserPassword[LOGIN_PASSWORD_LEN + 1];	// Login password cho user
	char EncryptKeyPublish[ENCRYPT_KEY_LEN + 1];// Encrype password publish
	char EncryptKeyPrivate[ENCRYPT_KEY_LEN + 1];// Encrype password private
	char WifiSsid[SSID_LEN + 1];	// WIFI WifiSsid
	char WifiPassword[WIFI_PASSWORD_LEN + 1];	// WIFI PASSWORD
	char WifiIP[IP_LEN + 1];	// static _IP
	char WifiMask[IP_LEN + 1];	// static WifiMask
	char WifiGW[IP_LEN + 1];	// static gateway
	bool DHCP;
	char APPassword[WIFI_PASSWORD_LEN + 1];	// AP WIFI PASSWORD
	char APIP[IP_LEN + 1];	// AP _IP
	char APMask[IP_LEN + 1];	// AP WifiMask
	char APGW[IP_LEN + 1];	// AP gateway

	int HTTPPort;	// Webserver port

	char MQTTBroker[MQTT_BROKER_LEN + 1];	// MQTT BROKER
	int MQTTPort;
	char MQTTUser[MQTT_USERNAME_LEN + 1];
	char MQTTPassword[MQTT_PASSWORD_LEN + 1];
	MQTTTopic_t MQTTTopics[TOPIC_SUB_MAX];

	char TimeServer[TIME_SERVER_LEN + 1];	// Server of internet time sync
	int TimeZone;

	BOutput outputs[OUTPUT_PINS];
	int ending;
} BSetting;

class DB: public Singleton<DB> {
public:
	DB();
	virtual ~DB();bool load();bool save();
	void reset();bool factory();bool resetPassword();
	// require function
	bool deviceName(const char * value);
	String getDeviceName();bool deviceType(const char * value);
	String getDeviceType();bool setDeviceSerial(const char * value);
	String getDeviceSerial();bool setLoginPassword(const char * value,
			bool isAdmin = false);
	String getLoginPassword(bool isAdmin = false);bool setSSID(
			const char * value, bool isAPMode = false);
	String getSSID(bool isAPMode = false);bool setWifiPassword(
			const char * value, bool isAPMode = false);
	String getWifiPassword(bool isAPMode = false);bool setIP(
			const char * value, bool isAPMode = false);
	String getIP(bool isAPMode = false);bool setMask(const char * value,
			bool isAPMode = false);
	String getMask(bool isAPMode = false);bool setGateway(
			const char * value, bool isAPMode = false);
	String getGateway(bool isAPMode = false);bool setDHCP(
			bool value = true);bool isDHCP();bool setTimeServer(
			const char * value);
	String getTimeServer();bool setTimeZone(int value);
	int getTimeZone();

	// Option module

	// HTTP Webserver module
	bool setHTTPPort(int port = 80);
	int getHTTPPort();

	// MQTT Client module
	bool setMQTTBroker(const char * value);
	String getMQTTBroker();bool setMQTTUsername(const char * value);
	String getMQTTUsername();bool setMQTTPassword(const char * value);
	String getMQTTPassword();bool setMQTTPort(int port = 1883);
	int getMQTTPort();
	MQTTTopic_t getTopic(int index);bool setTopic(int index, bool enable,
			const char* topicName);bool enableTopic(int index, bool value);

	// GPIO module
	bool setOutputName(int index, const char * value);
	String getOutputName(int index);bool setOutputType(int index,
			BOUTPUT_t type);
	BOUTPUT_t getOutputType(int index);bool setOutputTimeOn(int index,
			unsigned long time);
	unsigned long getOutputTimeOn(int index);bool setOutputTimeOff(
			int index, unsigned long time);
	unsigned long getOutputTimeOff(int index);
	int getOutputPin(int index);bool setOutputPin(int index, int pin);
	int getOutputFeedback(int index);bool setOutputFeedback(int index,
			int pin);

	// Encrypt module
	bool setEncryptKey(const char * value, bool isPrivate = false);
	String getEncryptKey(bool isPrivate = false);
protected:
	BSetting Data;bool isDirty();bool doLoad();bool doSave();bool clean();bool setTextField(
			const char * value, char *field, int maxLength);bool checkIntField(
			int value, bool checkMin = false, int minInt = 0,
			bool checkMax = false, int maxInt = 0);
	void updateMd5();
	String calcMd5();bool setupOutput(int index, const char *name,
			BOUTPUT_t type, int outputPin, int fbPin, unsigned long timeOn,
			unsigned long timeOff);
};

#endif

