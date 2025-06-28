#ifndef LED_TASKS_H
#define LED_TASKS_H

#include <Arduino.h>
#include <FastLED.h>


void taskLedsColorShift(void *pvParameters);
void taskLedsColorShiftGradient(void *pvParameters);
void taskLedsColorTrans(void *pvParameters);

void taskLedsBriCtrl(void *pvParameters);
void taskLedsBriBreathe(void *pvParameters);

void taskEyesBlink(void *pvParameters); // 眨眼睛任务 传入参数为LedEyes对象
void taskEyesBlinkTrigger(void *pvParameters);


#endif // LED_TASKS_H