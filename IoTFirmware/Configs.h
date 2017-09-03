/*
 * Configs.h
 *
 *  Created on: 25 thg 8, 2017
 *      Author: MyPC
 */

#ifndef CONFIGS_H_
#define CONFIGS_H_

#include <ESP8266WiFi.h>
#include <pgmspace.h>
#define _DEBUG_

#ifdef _DEBUG_
#define INIT_SERIAL_DEBUG() Serial.begin(115200)
#define DBG0(x) Serial.print(x)
#define DBGF0(x) Serial.print(F(x))
#define DBG(x) Serial.println(x)
#define DBG2(x,y) { Serial.print(x); Serial.println(y); }
#define DBGF(x) Serial.println(F(x))
#define DBG2F(x,y) { Serial.print(F(x)); Serial.println(y); }
#define DBG2F0(x,y) { Serial.print(F(x)); Serial.print(y); }
#else
#define INIT_SERIAL_DEBUG()
#define DBG0(x)
#define DBGF0(x)
#define DBG(x)
#define DBG2(x,y)
#define DBGF(x)
#define DBG2F(x,y)
#define DBG2F0(x,y)
#endif

/*
 * Global define - Nhung thiet lap di theo ban build
 */

#define HBOS_Version	"WSF.1.000"
#define HBOS_Server	"hbinvent.com"
#define HBOS_UPDATE_SERVER	"update.hbinvent.com"
#define HBOS_UPDATE_PATH	"/iotupdate/esp8266.php"


/*
 * Cac khai bao su dung cac module
 * */

#define	USE_TIMEZONE
#define USE_MQTT


/* Cau truc vung nho EEPROM:
 * 0 - 128: Device setting chua du lieu truy xuat nguon goc:
 * 			xem class DeviceSetting.h
 * 129 - 1xx: GPIO setting( xem class GPIOManager)
 *
 *
 */
#define	MAX_TIMEZONE_TABLE	79

/* Cau truc file setting luu tren FS (xem class ModuleSetting.h):
 *
 */

/*
Webserver
*/
#define TIMER_MAX_ALLOW	50
#define GZIP_ENABLE	1
#define GZIP_DISABLE	0
#define WIFI_CONN_TIME_OUT	30 //seconds

// Mang cac so nguyen to su dung cho Weekend timer
const int Week_u[8] ={1,2,3,5,7,11,13,17};

/////////////////////////////////////////////////////////////////////////////////////////
//int g_inputPins[INPUT_PIN_COUNT] = { 4, 5, 14 };
//int g_outputPins[OUTPUT_PIN_COUNT] = { 15 };
//int g_buttonPins[BUTTON_PIN_COUNT] = { 12, 13, 2 };


/*****************************************************************
 *****************************************************************
 ***********	Cac thiet lap cho GPIO		**********************
 *****************************************************************
 *****************************************************************/

#define MAX_GPIO_PIN	9
#define GPIO_NAME_MAX_LEN	32

#define RESET_PIN	0
#define	ANALOG_PIN	A0	//Analog input. Need to seed the random generator
#define	ARM_INPUT_PIN	5
#define	BELL_PIN	16
#define	ARM_OFF_PIN	99
#define GPIO_NAME_LEN	32

/*******************************************************************
Cac kieu cua GPIO OUTPUT:
	- IN
	- BLINK
	- TOGGLE
	- PWM
 ******************************************************************/
typedef enum BOUTPUT_t
{
	BLINK = 1,
	TOGGLE,
	PWM
};

/********************************************************************
 * 					Cac thong tin cua OUTPUT:
 *-------------------------------------------------------------------
 * 	NAME		|	TYPE			|	Description					|
 *-------------------------------------------------------------------
 * 	name		|	char*			|	Ten OUTPUT
 * 	type		|	BOUTPUT_t		|	Kieu OUTPUT
 * 	pin			|	int				|	PIN cua output
 * 	feedback	|	int				|	Feedback pin
 * 	timeOn		|	unsigned long	| Thoi gian ON (blink mode)
 * 	timeOff		|	unsigned long	| Thoi gian OFF (blink mode)
 *
 * *****************************************************************/
typedef struct __attribute__((packed)) BOutput_t{
	char name[GPIO_NAME_LEN + 1];
	BOUTPUT_t type;	// type of outputs
	int pin;	// GPIO number need control
	int feedback;	// GPIO number of feedback pin
	unsigned long timeOn;	// thoi gian bat ms, = 0 neu luon bat
	unsigned long timeOff;	// thoi gian tat ms, = 0 neu luon tat
} BOutput;
// Mang index lai thu tu GPIO
static const int GPIOList[MAX_GPIO_PIN] = { 16, 14, 12, 13, 15, 2, 0, 4, 5};


/*
 * Cac khai bao va dinh nghia cho Timer
 */


typedef struct __attribute__((packed)){
	int time;
	unsigned long weekend;
	int isEnable;
} GPIOTimer_t;

/*
 * Cac chuoi luu trong PROGMEM
 */


const char JsonTrue[] = "{\"status\":true}";
const char JsonFalse[] = "{\"status\":false}";
const char JsonTrueValue[] = "{\"value\":true}";
const char JsonFalseValue[] = "{\"value\":false}";


static const char MimeTypeJS[] = "application/javascript";
static const char MimeCss[] = "text/css";
static const char MimeHtml[] = "text/html";
static const char MimeJson[] = "application/json";
static const char MimePNG[] = "image/png";

static const char JsonStatusTrue[] = "{\"status\":true}";
static const char JsonStatusFalse[] = "{\"status\":false}";

static const char adminUser[] = "admin";

#endif /* CONFIGS_H_ */
