/*
 * EventHandle.cpp
 *
 *  Created on: 24 thg 8, 2017
 *      Author: MyPC
 */

#include "EventHandle.h"

EventHandle::EventHandle() {
	// TODO Auto-generated constructor stub
	BMQTT::instance()->onMessage(
			std::bind(&EventHandle::onMqttMessage,this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3,
			std::placeholders::_4,
			std::placeholders::_5,
			std::placeholders::_6));
}

EventHandle::~EventHandle() {
	// TODO Auto-generated destructor stub
}

void EventHandle::onMqttMessage(char* topic, char* payload,
		AsyncMqttClientMessageProperties properties, size_t len, size_t index,
		size_t total) {
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
}
