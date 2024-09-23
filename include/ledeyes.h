#ifndef LEDEYE_H
#define LEDEYE_H

#define NUM_LEDS 20
#define LED_L_PIN 16
#define LED_R_PIN 17

#include <Arduino.h>
#include <FastLED.h>

class LedEyes
{
private:

    CRGB leds_l[NUM_LEDS];
    CRGB leds_r[NUM_LEDS];
    
    const int LED_BRIGNTNESS_INIT = 128; // 0~255
    

public:
    LedEyes();       // 构造函数
    ~LedEyes();      // 析构函数
    void init();
    

};

#endif // LEDEYE_H
