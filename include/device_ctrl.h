#ifndef DEVICE_CTRL_H
#define DEVICE_CTRL_H

#include <Arduino.h>

#define FUN_CRIL_PIN 27
#define TEST_LED_G 32
#define TEST_LED_B 33

// 用于控制风扇开关和两个指示LED灯
void deviceSetup();
void funEnable();
void funDisable();

#endif // DEVICE_CTRL_H
