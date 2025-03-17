#ifndef DEVICE_CTRL_H
#define DEVICE_CTRL_H

#include <Arduino.h>

#define FUN_CRIL_PIN 27
#define TEST_LED_G 32
#define TEST_LED_B 33

extern bool led_B_tigger;

// 用于控制风扇开关和两个指示LED灯
void deviceSetup();
void funEnable();
void funDisable();
void taskLedBlueBlink(void *pvParameters);

#endif // DEVICE_CTRL_H
