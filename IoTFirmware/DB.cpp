#include "DB.h"


static const char default_apiKey[] = "db67b42f-96ea-49a0-aacb-3ab958cdeadc";
static const char default_SMSApiKey[] = "f8bdca74";
static const char default_SMSApiSecret[] = "f00c543ee476ad85";
DB::DB() {
	DBG2F("EEPROM size:  ", sizeof(BSetting));
}

DB::~DB() {
}

bool DB::load() {
	doLoad();
	if (isDirty() || Data.ending != DEF_DATA_ENDING) {
		DBGF("Invalid EEPROM data. Load default setting instead.");
		return factory();
	}
	DBG2F("Checksum: ", Data.MD5);
	DBG2F("Name: ", Data.Name);
	DBG2F("Type: ", Data.Type);
	DBG2F("Serial: ", Data.Serial);
	DBG2F("AdminPassword: ", Data.AdminPassword);
	DBG2F("UserPassword: ", Data.UserPassword);
	DBG2F("EncryptKeyPublish: ", Data.EncryptKeyPublish);
	DBG2F("EncryptKeyPrivate: ", Data.EncryptKeyPrivate);
	DBG2F("WifiSsid: ", Data.WifiSsid);
	DBG2F("WifiPassword: ", Data.WifiPassword);
	DBG2F("WifiIP: ", Data.WifiIP);
	DBG2F("WifiMask: ", Data.WifiMask);
	DBG2F("WifiGW: ", Data.WifiGW);
	DBG2F("DHCP: ", Data.DHCP);
	DBG2F("APPassword: ", Data.APPassword);
	DBG2F("APIP: ", Data.APIP);
	DBG2F("APMask: ", Data.APMask);
	DBG2F("APGW: ", Data.APGW);
	DBG2F("HTTPPort: ", Data.HTTPPort);
	DBG2F("MQTTBroker: ", Data.MQTTBroker);
	DBG2F("MQTTPort: ", Data.MQTTPort);
	DBG2F("MQTTUser: ", Data.MQTTUser);
	DBG2F("MQTTPassword: ", Data.MQTTPassword);
	DBG2F("MQTTTopics 0: ", Data.MQTTTopics[0].topic);
	DBG2F("MQTTTopics 0 enable: ", Data.MQTTTopics[0].enable);
	DBG2F("TimeServer: ", Data.TimeServer);
	DBG2F("TimeZone: ", Data.TimeZone);
	for (int i = 0; i < OUTPUT_PINS; i++) {
		DBG2F0("outputs ", i + 1);
		DBG2F0("\t GPIO: ", Data.outputs[i].pin);
		DBG2F0("\t feedback: ", Data.outputs[i].feedback);
		DBG2F0("\t name: ", Data.outputs[i].name);
		DBG2F0("\t timeOn: ", Data.outputs[i].timeOn);
		DBG2F("\t timeOff: ", Data.outputs[i].timeOff);
		yield();
	}
	DBG2F("ending: ", Data.ending);
	return true;
}

bool DB::save() {
	if (!isDirty()) {
		DBGF("Nothing to save as no change");
		return true;
	}
	clean();
	updateMd5();
	return doSave();
}

void DB::reset() {
	int i;
	String mac = WiFi.softAPmacAddress();
	mac.replace(':', '_');
	memset(&Data, 0, sizeof(BSetting));	// set Data to zero
	Data.MD5[0] = 0;
	deviceName(mac.c_str());
	setDeviceSerial("WS1-00000000001");

	MQTTTopic_t topic;
	topic.enable = true;
	strncpy(topic.topic, "binhpham", TOPIC_SUB_LEN);
	Data.MQTTTopics[0] = topic;
	for (int i = 1; i < TOPIC_SUB_MAX; i++) {
		setTopic(i, false, "");
	}
	setLoginPassword(DEF_ADMIN_PASSWORD, ADMIN_SETTING);
	setLoginPassword(DEF_USER_PASSWORD);
	setEncryptKey(DEF_ENCRYTKEY_PRIVATE, KEY_PRIVATE);
	setEncryptKey(DEF_ENCRYTKEY_PUBLIC);
	setSSID(DEF_SSID_STA);
	setWifiPassword(DEF_PASSWORD_AP, AP_SETTING);
	setWifiPassword(DEF_PASSWORD_STA);
	setIP(DEF_IP_AP, AP_SETTING);
	setIP(DEF_IP_STA);
	setMask(DEF_MASK_AP, AP_SETTING);
	setMask(DEF_MASK_STA);
	setGateway(DEF_GW_AP, AP_SETTING);
	setGateway(DEF_GW_STA);
	setDHCP(true);
	setHTTPPort(80);
	setMQTTBroker(DEF_MQTT_BROKER);
	setMQTTUsername(DEF_MQTT_USER);
	setMQTTPassword(DEF_MQTT_PASSWORD);
	setMQTTPort(1883);
	setTimeServer(DEF_TIME_SERVER);
	setTimeZone(0);
	setupOutput(0, "output 1", TOGGLE, OUTPUT_PIN_1, OUTPUT_FB_1, 0, 0);
	setupOutput(1, "output 2", TOGGLE, OUTPUT_PIN_2, OUTPUT_FB_2, 0, 0);
	setupOutput(2, "output 3", TOGGLE, OUTPUT_PIN_3, OUTPUT_FB_3, 0, 0);
	setupOutput(3, "output 4", TOGGLE, OUTPUT_PIN_4, OUTPUT_FB_4, 0, 0);
	setupOutput(4, "output 5", TOGGLE, OUTPUT_PIN_5, OUTPUT_FB_5, 0, 0);
	Data.ending = DEF_DATA_ENDING;
}

bool DB::factory() {
	clean();
	reset();
	updateMd5();
	doSave();
	return doLoad();
}

bool DB::deviceName(const char * value) {
	return setTextField(value, Data.Name, DEVICE_NAME_LEN);
}

String DB::getDeviceName() {
	return Data.Name;
}

bool DB::deviceType(const char * value) {
	return setTextField(value, Data.Type, DEVICE_TYPE_LEN);
}

String DB::getDeviceType() {
	return Data.Type;
}

bool DB::setDeviceSerial(const char * value) {
	return setTextField(value, Data.Serial, DEVICE_SERIAL_LEN);
}

String DB::getDeviceSerial() {
	return Data.Serial;
}

bool DB::setLoginPassword(const char * value, bool isAdmin) {
	if (isAdmin) {
		return setTextField(value, Data.AdminPassword, LOGIN_PASSWORD_LEN);
	} else {
		return setTextField(value, Data.UserPassword, LOGIN_PASSWORD_LEN);
	}
	return false;
}

String DB::getLoginPassword(bool isAdmin) {
	if (isAdmin) {
		return Data.AdminPassword;
	} else {
		return Data.UserPassword;
	}
}

bool DB::setEncryptKey(const char * value, bool isPrivate) {
	if (isPrivate) {
		return setTextField(value, Data.EncryptKeyPrivate, ENCRYPT_KEY_LEN);
	} else {
		return setTextField(value, Data.EncryptKeyPublish, ENCRYPT_KEY_LEN);
	}
	return false;
}

String DB::getEncryptKey(bool isPrivate) {
	if (isPrivate) {
		return Data.EncryptKeyPrivate;
	} else {
		return Data.EncryptKeyPublish;
	}
}

bool DB::setSSID(const char * value, bool isAPMode) {
	if (!isAPMode) {
		return setTextField(value, Data.WifiSsid, SSID_LEN);
	}
	return false;
}

String DB::getSSID(bool isAPMode) {
	if (isAPMode) {
		return Data.Serial;
	} else {
		return Data.WifiSsid;
	}
}

bool DB::setWifiPassword(const char * value, bool isAPMode) {
	if (isAPMode) {
		return setTextField(value, Data.APPassword, WIFI_PASSWORD_LEN);
	} else {
		return setTextField(value, Data.WifiPassword, WIFI_PASSWORD_LEN);
	}
	return false;
}

String DB::getWifiPassword(bool isAPMode) {
	if (isAPMode) {
		return Data.APPassword;
	} else {
		return Data.WifiPassword;
	}
}

bool DB::setIP(const char * value, bool isAPMode) {
	if (isAPMode) {
		return setTextField(value, Data.APIP, IP_LEN);
	} else {
		return setTextField(value, Data.WifiIP, IP_LEN);
	}
	return false;
}

String DB::getIP(bool isAPMode) {
	if (isAPMode) {
		return Data.APIP;
	} else {
		return Data.WifiIP;
	}
}

bool DB::setMask(const char * value, bool isAPMode) {
	if (isAPMode) {
		return setTextField(value, Data.APMask, IP_LEN);
	} else {
		return setTextField(value, Data.WifiMask, IP_LEN);
	}
	return false;
}

String DB::getMask(bool isAPMode) {
	if (isAPMode) {
		return Data.APMask;
	} else {
		return Data.WifiMask;
	}
}

bool DB::setGateway(const char * value, bool isAPMode) {
	if (isAPMode) {
		return setTextField(value, Data.APGW, IP_LEN);
	} else {
		return setTextField(value, Data.WifiGW, IP_LEN);
	}
	return false;
}

String DB::getGateway(bool isAPMode) {
	if (isAPMode) {
		return Data.APGW;
	} else {
		return Data.WifiGW;
	}
}

bool DB::setDHCP(bool value) {
	Data.DHCP = value;
	return true;
}

bool DB::isDHCP() {
	return Data.DHCP;
}

bool DB::setHTTPPort(int port) {
	if (checkIntField(port, true, 1)) {
		Data.HTTPPort = port;
		return true;
	}
	return false;
}

int DB::getHTTPPort() {
	return Data.HTTPPort;
}

bool DB::setMQTTBroker(const char * value) {
	return setTextField(value, Data.MQTTBroker, MQTT_BROKER_LEN);
}

String DB::getMQTTBroker() {
	return Data.MQTTBroker;
}

bool DB::setMQTTUsername(const char * value) {
	return setTextField(value, Data.MQTTUser, MQTT_USERNAME_LEN);
}

String DB::getMQTTUsername() {
	return Data.MQTTUser;
}

bool DB::setMQTTPassword(const char * value) {
	return setTextField(value, Data.MQTTPassword, MQTT_PASSWORD_LEN);
}

String DB::getMQTTPassword() {
	return Data.MQTTPassword;
}

bool DB::setMQTTPort(int port) {
	if (checkIntField(port, true, 1)) {
		Data.MQTTPort = port;
		return true;
	}
	return false;
}

int DB::getMQTTPort() {
	return Data.MQTTPort;
}

bool DB::setTimeServer(const char * value) {
	return setTextField(value, Data.TimeServer, TIME_SERVER_LEN);
}

String DB::getTimeServer() {
	return Data.TimeServer;
}

bool DB::setTimeZone(int value) {
	if (checkIntField(value, true, 0, true, MAX_TIMEZONE_TABLE)) {
		Data.TimeZone = value;
		return true;
	}
	return false;
}

int DB::getTimeZone() {
	return Data.TimeZone;
}

bool DB::setOutputName(int index, const char * value) {
	return setTextField(value, Data.outputs[index].name, GPIO_NAME_LEN);
}

String DB::getOutputName(int index) {
	return Data.outputs[index].name;
}

bool DB::setOutputType(int index, BOUTPUT_t type) {
	Data.outputs[index].type = type;
	return true;
}

BOUTPUT_t DB::getOutputType(int index) {
	return Data.outputs[index].type;
}

bool DB::setOutputTimeOn(int index, unsigned long time) {
	if (index >= OUTPUT_PINS || index < 0) {
		return false;
	}
	Data.outputs[index].timeOn = time;
	return true;
}

unsigned long DB::getOutputTimeOn(int index) {
	return Data.outputs[index].timeOn;
}

bool DB::setOutputTimeOff(int index, unsigned long time) {
	if (index >= OUTPUT_PINS || index < 0) {
		return false;
	}
	Data.outputs[index].timeOff = time;
	return true;
}

unsigned long DB::getOutputTimeOff(int index) {
	return Data.outputs[index].timeOff;
}

int DB::getOutputPin(int index) {
	if (index >= OUTPUT_PINS || index < 0) {
		return -1;
	}
	return Data.outputs[index].pin;
}

bool DB::setOutputPin(int index, int pin) {
	if (index >= OUTPUT_PINS || index < 0) {
		return false;
	}
	Data.outputs[index].pin = pin;
	return true;
}

int DB::getOutputFeedback(int index) {
	if (index >= OUTPUT_PINS || index < 0) {
		return -1;
	}
	return Data.outputs[index].feedback;
}

bool DB::setOutputFeedback(int index, int pin) {
	if (index >= OUTPUT_PINS || index < 0) {
		return false;
	}
	Data.outputs[index].feedback = pin;
	return true;
}

MQTTTopic_t DB::getTopic(int index) {
	return Data.MQTTTopics[index];
}

bool DB::setTopic(int index, bool enable, const char* topicName) {
	MQTTTopic_t newTopic;
	newTopic.enable = enable;
	strncpy(newTopic.topic, topicName, TOPIC_SUB_LEN);
	Data.MQTTTopics[index] = newTopic;
	return true;
}

bool DB::enableTopic(int index, bool value) {
	Data.MQTTTopics[index].enable = value;
	return true;
}

bool DB::isDirty() {
	DBG2F("MD5 of Settings: ", Data.MD5);
	DBG2F("Calculated MD5:  ", calcMd5());
	return calcMd5() != Data.MD5;
}

bool DB::doLoad() {
	DBGF("Loading setting from EEPROM");
	int n = sizeof(BSetting);
	uint8_t *p = (uint8_t*) (&Data);
	EEPROM.begin(n);
	for (int i = SETTING_BLOCK_START; i < n + SETTING_BLOCK_START; i++) {
		yield();
		*p = EEPROM.read(i);
		p++;
	}
	EEPROM.end();
	return true;
}

bool DB::doSave() {
	DBGF("do save Settings.");
	int n = sizeof(BSetting);
	uint8_t *p = (uint8_t*) (&Data);
	EEPROM.begin(n);
	for (int i = SETTING_BLOCK_START; i < n + SETTING_BLOCK_START; i++) {
		yield();
		EEPROM.write(i, *p);
		p++;
	}
	bool bRvl = EEPROM.commit();
	if (bRvl) {
		DBGF("Settings saved succesfully.");
	} else {
		DBGF("Error saving settings.");
	}
	EEPROM.end();
	return bRvl;
}

bool DB::clean() {
	DBGF("Cleaning EEPROM.");
	int n = sizeof(BSetting);
	EEPROM.begin(n);
	for (int i = SETTING_BLOCK_START; i < n + SETTING_BLOCK_START; i++) {
		yield();
		EEPROM.write(i, 0);
	}
	bool bRvl = EEPROM.commit();
	if (bRvl) {
		DBGF("Clean succesfully.");
	} else {
		DBGF("Error Clean settings.");
	}
	EEPROM.end();
	return bRvl;
}

bool DB::setTextField(const char * value, char *field, int maxLength) {
	if (value == NULL || strlen(value) > maxLength)
		return false;
	strcpy(field, value);
	return true;
}

bool DB::checkIntField(int value, bool checkMin, int minInt, bool checkMax,
		int maxInt) {
	if (checkMin) {
		if (value < minInt) {
			return false;
		}
	}
	if (checkMax) {
		if (value > maxInt) {
			return false;
		}
	}
	return true;
}

void DB::updateMd5() {
	strncpy(Data.MD5, calcMd5().c_str(), CHECKSUM_LEN);
}

String DB::calcMd5() {
	MD5Builder builder;
	int MD5len = sizeof(Data.MD5);
	uint8_t *p = (uint8_t*) (&Data);
	p += MD5len;
	yield();
	builder.begin();
	builder.add(p, sizeof(BSetting) - MD5len);
	builder.calculate();
	yield();
	return builder.toString();
}

bool DB::setupOutput(int index, const char * name, BOUTPUT_t type,
		int outputPin, int fbPin, unsigned long timeOn, unsigned long timeOff) {
	BOutput output;
	strncpy(output.name, name, GPIO_NAME_LEN);
	output.type = type;
	output.pin = outputPin;
	output.feedback = fbPin;
	output.timeOn = timeOn;
	output.timeOff = timeOff;
	Data.outputs[index] = output;
	return false;
}

bool DB::resetPassword() {
	setLoginPassword(DEF_ADMIN_PASSWORD, ADMIN_SETTING);
	setLoginPassword(DEF_USER_PASSWORD);
	save();
}
