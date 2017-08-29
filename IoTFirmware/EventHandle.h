/*
 * EventHandle.h
 *
 *  Created on: 24 thg 8, 2017
 *      Author: MyPC
 */

#ifndef EVENTHANDLE_H_
#define EVENTHANDLE_H_

#include <vector>
#include <functional>
#include "Singleton.h"
#include "BMQTT.h"
/********************************************************
 * Cac event se duoc lang nghe
 *
 * */
typedef enum BEvent_t{
	WIFI_CONNECT = 1,
	WIFI_DISCONNECT,
	MQTT_NEW_PAYLOAD
} BEvent;

/********************************************************
 * cac con tro ham dung cho cac event
 *
 * */
typedef std::function<void(void)> wifiEvtCallback;
typedef std::function<void (char*, char*)> mqttPayloadCallback;

class EventHandle: public Singleton<EventHandle> {
public:
	EventHandle();
	virtual ~EventHandle();
private:
	void onMqttMessage(char * topic, char * payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
};

#endif /* EVENTHANDLE_H_ */
